#include <iostream>
#include "Halide.h"

using namespace Halide;

Var x("x"), y("y"), z("z"), c("c");
Var x_grid("x_grid"), y_grid("y_grid"), xo("xo"), yo("yo"), x_in("x_in"), y_in("y_in");

int windowR = 4;
int searchR = 64;
//int windowR = 8;
//int searchR = 120;

Func rectify_float(Func img, Func remap) {
    Expr offsetX = cast<float>(cast<int16_t>(remap(x, y, 0)) - cast<int16_t>(128)) / 16.0f;
    Expr offsetY = cast<float>(cast<int16_t>(remap(x, y, 1)) - cast<int16_t>(128)) / 16.0f;

    Expr targetX = cast<int>(floor(offsetX));
    Expr targetY = cast<int>(floor(offsetY));

    Expr wx = offsetX - targetX;
    Expr wy = offsetY - targetY;

    Func interpolated("interpolated");
    interpolated(x, y) = lerp(lerp(img(x+targetX, y+targetY, 1), img(x+targetX+1, y+targetY, 1), wx),
                              lerp(img(x+targetX, y+targetY+1, 1), img(x+targetX+1, y+targetY+1, 1), wx), wy);

    return interpolated;
}

Func rectify_int(Func img, Func remap) {
    Expr targetX = (cast<int16_t>(remap(x, y, 0)) - 128) / 16;
    Expr targetY = (cast<int16_t>(remap(x, y, 1)) - 128) / 16;

    // wx and wy are equal to wx*256 and wy*256 in rectify_float()
    Expr wx = cast<uint8_t>((cast<int16_t>(remap(x, y, 0)) * 16) - (128 * 16) - (targetX * 256));
    Expr wy = cast<uint8_t>((cast<int16_t>(remap(x, y, 1)) * 16) - (128 * 16) - (targetY * 256));

    Func interpolated("interpolated");
    interpolated(x, y) = lerp(lerp(img(x+targetX, y+targetY, 1), img(x+targetX+1, y+targetY, 1), wx),
                              lerp(img(x+targetX, y+targetY+1, 1), img(x+targetX+1, y+targetY+1, 1), wx), wy);

    return interpolated;
}

Func rectify_noop(Func img, Func remap) {
    Func pass("pass");
    pass(x, y) = img(x, y, 1);
    return pass;
}

class HalidePipeline {
public:
    ImageParam left, right, left_remap, right_remap;
    Func left_padded, right_padded, left_remap_padded, right_remap_padded;
    Func left_remapped, right_remapped;
    Func SAD, offset, output, hw_output;
    RDom win, search;
    std::vector<Argument> args;

    HalidePipeline()
        : left(UInt(8), 3), right(UInt(8), 3),
          left_remap(UInt(8), 3), right_remap(UInt(8), 3),
          SAD("SAD"), offset("offset"), output("output"), hw_output("offload"),
          win(-windowR, windowR*2, -windowR, windowR*2),
          search(0, searchR)
    {
        right_padded = BoundaryConditions::constant_exterior(right, 0);
        left_padded = BoundaryConditions::constant_exterior(left, 0);
        right_remap_padded = BoundaryConditions::constant_exterior(right_remap, 128);
        left_remap_padded = BoundaryConditions::constant_exterior(left_remap, 128);

        right_remapped = rectify_noop(right_padded, right_remap_padded);
        left_remapped = rectify_noop(left_padded, left_remap_padded);

        SAD(x, y, c) += cast<uint16_t>(absd(right_remapped(x+win.x, y+win.y),
                                            left_remapped(x+win.x+20+c, y+win.y)));

        //offset(x, y) = argmin(SAD(x, y, search.x));
        // avoid using the form of the inlined reduction function of "argmin",
        // so that we can get a handle for scheduling
        offset(x, y) = {cast<int8_t>(0), cast<uint16_t>(65535)};
        offset(x, y) = {select(SAD(x, y, search.x) < offset(x, y)[1],
                               cast<int8_t>(search.x),
                               offset(x, y)[0]),
                        min(SAD(x, y, search.x), offset(x, y)[1])};

        hw_output(x, y) = cast<uint8_t>(cast<uint16_t>(offset(x, y)[0]) * 255 / searchR);
        output(x, y) = hw_output(x, y);

        // all inputs has three channels
        right.set_bounds(2, 0, 3);
        left.set_bounds(2, 0, 3);
        right_remap.set_bounds(2, 0, 3);
        left_remap.set_bounds(2, 0, 3);

        // Arguments
        args = {right, left, right_remap, left_remap};

    }

    void compile_to_cpu() {
        /*output.tile(x, y, xo, yo, x_in, y_in, 5, 5);
        hw_output.tile(x, y, xo, yo, x_in, y_in, 5, 5);

        right_remapped.compute_at(hw_output, xo);
        left_remapped.compute_at(hw_output, xo);

        hw_output.compute_at(output, xo);*/

        output.compile_to_lowered_stmt("ir.cpu.html", args, HTML);
        output.compile_to_c("cpu.cpp", args, "cpu");
        output.compile_to_header("cpu.h", args, "cpu");
    }

    void compile_to_hls() {
        output.tile(x, y, xo, yo, x_in, y_in, 256, 256, TailStrategy::RoundUp);
        hw_output.tile(x, y, xo, yo, x_in, y_in, 256, 256, TailStrategy::RoundUp);


        hw_output.compute_at(output, xo);
        right_remapped.compute_at(output, xo);
        left_remapped.compute_at(output, xo);

        hw_output.offload({}, xo);

        //Maybe it is some kind of cheating, but I have no other ideas to fix it.
        //I will tell Prof. later.
        //SAD.unroll(c);
        //SAD.update(0).unroll(win.x).unroll(win.y).unroll(c);

        output.compile_to_lowered_stmt("ir.hls.html", args, HTML);
        output.compile_to_sdsoc("top", args, "top");
    }

};

// Optimize for hls code generation
// We change the algorithm to generated reduction tree for unrolling argmin
class MyPipelineOpt {
public:
ImageParam left, right, left_remap, right_remap;
Func left_padded, right_padded, left_remap_padded, right_remap_padded;
Func left_remapped, right_remapped;
Func SAD, offset, offset_l1, output, hw_output;
RDom win, search_l1, search;
std::vector<Argument> args;

MyPipelineOpt()
    : left(UInt(8), 3), right(UInt(8), 3),
      left_remap(UInt(8), 3), right_remap(UInt(8), 3),
      SAD("SAD"), offset("offset"), output("output"), hw_output("offload"),
      win(-windowR, windowR*2, -windowR, windowR*2),
      search_l1(0, 4), search(0, searchR/4)
{
    right_padded = BoundaryConditions::constant_exterior(right, 0);
    left_padded = BoundaryConditions::constant_exterior(left, 0);
    right_remap_padded = BoundaryConditions::constant_exterior(right_remap, 128);
    left_remap_padded = BoundaryConditions::constant_exterior(left_remap, 128);

    right_remapped = rectify_noop(right_padded, right_remap_padded);
    left_remapped = rectify_noop(left_padded, left_remap_padded);

    SAD(x, y, c) += cast<uint16_t>(absd(right_remapped(x+win.x, y+win.y),
                                        left_remapped(x+win.x+20+c, y+win.y)));

    /*
    offset(x, y) = argmin(SAD(x, y, search.x));
    */
    // offset_l1 caculates {minarg, minval} betwen SAD(x, y, c*4) and SAD(x, y, c*4+3)
    offset_l1(x, y, c) = {cast<int8_t>(0), cast<uint16_t>(65535)};
    offset_l1(x, y, c) = {select(SAD(x, y, c*4 + search_l1.x) < offset_l1(x, y, c)[1],
                                 cast<int8_t>(c*4 + search_l1.x),
                                      offset_l1(x, y, c)[0]),
                                 min(SAD(x, y, c*4 + search_l1.x), offset_l1(x, y, c)[1])};

    offset(x, y) = {cast<int8_t>(0), cast<uint16_t>(65535)};
    offset(x, y) = {select(offset_l1(x, y, search.x)[1] < offset(x, y)[1],
                           offset_l1(x, y, search.x)[0],
                           offset(x, y)[0]),
                           min(offset_l1(x, y, search.x)[1], offset(x, y)[1])};

    hw_output(x, y) = cast<uint8_t>(cast<uint16_t>(offset(x, y)[0]) * 255 / searchR);
    output(x, y) = hw_output(x, y);

    // The comment constraints and schedules.
    // all inputs has three channels
    right.set_bounds(2, 0, 3);
    left.set_bounds(2, 0, 3);
    right_remap.set_bounds(2, 0, 3);
    left_remap.set_bounds(2, 0, 3);

    /*
    Expr out_width = output.output_buffer().width();
    Expr out_height = output.output_buffer().height();
    output
        .bound(x, 0, (out_width/600)*600)
        .bound(y, 0, (out_height/400)*400);
    */

    // Arguments
    args = {right, left, right_remap, left_remap};
}


void compile_to_hls() {
    std::cout << "\ncompiling HLS code..." << std::endl;

    output.tile(x, y, xo, yo, x_in, y_in, 480, 640, TailStrategy::RoundUp);
    right_remapped.compute_at(output, xo);
    left_remapped.compute_at(output, xo);
    //right_padded.compute_at(hw_output, xo);
    //left_padded.compute_at(hw_output, xo);
    //right_remap_padded.compute_at(hw_output, xo);
    //left_remap_padded.compute_at(hw_output, xo);

    hw_output.compute_at(output, xo);
    hw_output.tile(x, y, xo, yo, x_in, y_in, 480, 640);

    //offset.update(0).unroll(search.x, 4);
    //RVar search_xo, search_xi;
    //offset.update(0).split(search.x, search_xo, search_xi, 4);
    //offset.update(0).unroll(search_xi);
    //SAD.compute_at(offset_l1, Var::outermost());
    //SAD.unroll(c);
    SAD.update(0).unroll(win.x).unroll(win.y).unroll(c);
    //offset_l1.compute_at(offset, search_xo);
    //offset_l1.unroll(c);
    //offset_l1.update(0).unroll(c).unroll(search_l1.x);

    hw_output.offload({}, xo);

    output.compile_to_lowered_stmt("ir.hls.html", args, HTML);
    output.compile_to_sdsoc("top", args, "top");
}
};

int main(int argc, char *argv[]) {
    if (argc != 1 && argc != 2) {
        std::cerr << "Usage: ./generator <target>\n";
        std::cerr << "By default, it is targetted to native CPU code.\n";
        return 1;
    }

    if (argc == 1 || !strcmp(argv[1], "CPU")) {
        HalidePipeline().compile_to_cpu();
    } else if (!strcmp(argv[1], "HLS")) {
        //MyPipelineOpt().compile_to_hls();
        HalidePipeline().compile_to_hls();
    }
    return 0;
}

