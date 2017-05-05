#include <iostream>
#include "Halide.h"

using namespace Halide;

struct MyPipeline {

    ImageParam input, weight;
    Param<uint8_t> bias;

    Var c, x, y, xo, yo, xi, yi;
    RDom r1, r2;

    Func prepare, conv, relu, offload, output;

    std::vector<Argument> args;

    MyPipeline() : 
        input(type_of<uint8_t>(), 3, "input"), 
        weight(type_of<uint8_t>(), 4, "weight"),
        bias("bias"),
        c("c"), x("x"), y("y"), xo("xo"), yo("yo"), xi("xi"), yi("yi"),
        r1(0, 3, 0, 3, 0, 3), r2(0, 2, 0, 2),
        prepare("prepare"), conv("conv"), relu("relu"), offload("offload"), output("output"),
        args{input, weight, bias}
    {
        prepare = BoundaryConditions::repeat_edge(input);

        conv(c, x, y) = 
            (sum(prepare(r1.x, x + r1.x - 1, y + r1.y) + weight(c, r1.x, r1.y, r1.z)) + bias) >> 8;

        relu(c, x, y) = max(0, conv(c, x, y));
        offload(c, x, y) = maximum(relu(c, x + r2.x, y + r2.x));
        output(c, x, y) = offload(c, x, y);
        weight.dim(0).set_bounds(0, 64);
        weight.dim(1).set_bounds(0, 3);
        weight.dim(2).set_bounds(0, 3);
        weight.dim(3).set_bounds(0, 3);
        output.bound(c, 0, 64);
    }

    void compile_to_cpu() {
        output.compile_to_file("cnn", args, "cnn");
        output.compile_to_lowered_stmt("ir.cpu.html", args, HTML);
    }

    void compile_to_hls() {
        output.tile(x, y, xo, yo, xi, yi, 64, 64);
        prepare.compute_at(output, xo);
        offload.tile(x, y, xo, yo, xi, yi, 64, 64);
        offload.compute_at(output, xo);
        //conv.compute_at(offload, xo);
        offload.offload({conv}, xo);
        output.compile_to_lowered_stmt("ir.hls.html", args, HTML);
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

