#include <iostream>
#include "Halide.h"

using namespace Halide;

struct MyPipeline {

    ImageParam input, weight;

    Var c, x, y, xo, yo, xi, yi;
    RDom r;

    Func prepare, conv, offload, output;

    std::vector<Argument> args;

    MyPipeline() : 
        input(type_of<uint8_t>(), 3, "input"), 
        weight(type_of<uint8_t>(), 3, "weight"),
        c("c"), x("x"), y("y"), xo("xo"), yo("yo"), xi("xi"), yi("yi"),
        r(-2, 5, -2, 5),
        prepare("prepare"), conv("conv"), offload("offload"), output("output"),
        args{input, weight}
    {
        prepare = BoundaryConditions::repeat_edge(input);

        offload(c, x, y) = 
            cast<int32_t>(sum(cast<int32_t>(prepare(x + r.x, y + r.y, c)) * cast<int32_t>(weight(r.x + 2, r.y + 2, c)))) >> 8;

        output(x, y, c) = offload(c, x, y);

        weight.dim(0).set_bounds(0, 5);
        weight.dim(1).set_bounds(0, 5);
        weight.dim(2).set_bounds(0, 3);
    }

    void compile_to_cpu() {
        //output.compile_to_file("conv", args, "conv");
        /*prepare.compute_at(output, xo);
        offload.tile(x, y, xo, yo, xi, yi, 640, 480);
        offload.compute_at(output, xo);
        conv.compute_at(offload, x);
        output.compile_to_lowered_stmt("ir.cpu.html", args, HTML);*/
        output.compile_to_c("cpu.cpp", args, "cpu");
        output.compile_to_header("cpu.h", args, "cpu");
        output.compile_to_lowered_stmt("ir.cpu.html", args, HTML);
    }

    void compile_to_hls() {
        output.tile(x, y, xo, yo, xi, yi, 128, 128);
        offload.tile(x, y, xo, yo, xi, yi, 128, 128);
        offload.bound(c, 0, 3);
        prepare.compute_at(output, xo);
        offload.compute_at(output, xo);
        offload.offload({}, xo).unroll(c);
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

    //Compile to IR, HLS, or CPU
    if (argc == 1 || !strcmp(argv[1], "CPU")) {
        MyPipeline().compile_to_cpu();
    } else if (!strcmp(argv[1], "HLS")) {
        MyPipeline().compile_to_hls();
    } else {
        std::cerr << "Unknown target!\n";
        return 1;
    }
    
    std::cout << "Generated Conv_gen\n";
    return 0;
}

