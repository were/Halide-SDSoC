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
        weight(type_of<uint8_t>(), 2, "weight"),
        c("c"), x("x"), y("y"), xo("xo"), yo("yo"), xi("xi"), yi("yi"),
        r(-2, 5, -2, 5),
        prepare("prepare"), conv("conv"), offload("offload"), output("output"),
        args{input, weight}
    {
        prepare = BoundaryConditions::repeat_edge(input);

        conv(x, y, c) = 
            (sum(prepare(x + r.x, y + r.y, c) * weight(r.x + 2, r.y + 2))) >> 8;
        //conv.unroll(c);

        offload(x, y, c) = 
            (sum(conv(x + r.x, y + r.y, c) * weight(r.x + 2, r.y + 2))) >> 8;
        //offload.unroll(c);

        output(x, y, c) = offload(x, y, c);

        weight.dim(0).set_bounds(0, 5);
        weight.dim(1).set_bounds(0, 5);
        output.bound(c, 0, 3);
    }

    void compile_to_cpu() {
        output.compile_to_file("conv", args, "conv");
	    output.compile_to_lowered_stmt("ir.cpu.html", args, HTML);
    }

    void compile_to_hls() {
        output.tile(x, y, xo, yo, xi, yi, 640, 480);
        prepare.compute_at(output, xo);
        offload.tile(x, y, xo, yo, xi, yi, 640, 480);
        offload.compute_at(output, xo);
        offload.offload({conv}, xo);
        offload.reorder(c, xi, yi, xo, yo).unroll(c);
        output.reorder(c, xi, yi, xo, yo);
        conv.reorder(c, x, y).unroll(c);
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

