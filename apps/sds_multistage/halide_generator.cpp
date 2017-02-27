#include <iostream>
#include "Halide.h"

using namespace Halide;

struct MyPipeline {

    ImageParam input, weight;

    Var c, x, y, xo, yo, xi, yi;
    RDom r;

    Func prepare, conv, conv1, offload, output;

    std::vector<Argument> args;

    MyPipeline() : 
        input(type_of<uint8_t>(), 2, "input"), 
        weight(type_of<uint8_t>(), 2, "weight"),
        c("c"), x("x"), y("y"), xo("xo"), yo("yo"), xi("xi"), yi("yi"),
        r(-2, 5, -2, 5),
        prepare("prepare"), conv("conv"), conv1("conv1"), 
        offload("offload"), output("output"),
        args{input, weight}
    {
        prepare = BoundaryConditions::repeat_edge(input);

        conv(x, y) += cast<int32_t>(prepare(x + r.x, y + r.y) * weight(r.x + 2, r.y + 2));
        conv(x, y) = conv(x, y) >> 8;

        conv1(x, y) += cast<int32_t>(conv(x + r.x, y + r.y) * weight(r.x + 2, r.y + 2));
        conv1(x, y) = conv1(x, y) >> 8;

        offload(x, y) = conv1(x, y);

        output(x, y) = offload(x, y);

        weight.dim(0).set_bounds(0, 5);
        weight.dim(1).set_bounds(0, 5);
    }

    void compile_to_cpu() {
        output.compile_to_file("conv", args, "conv");
        prepare.compute_at(output, xo);
	    output.compile_to_lowered_stmt("ir.cpu.html", args, HTML);
    }

    void compile_to_hls() {
        output.tile(x, y, xo, yo, xi, yi, 640, 480);
        offload.tile(x, y, xo, yo, xi, yi, 640, 480);
        prepare.compute_at(output, xo);
        offload.compute_at(output, xo);
        offload.offload({conv, conv1}, xo);
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
