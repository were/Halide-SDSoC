#include <iostream>
#include "Halide.h" 
using namespace Halide;

struct HalidePipeline {
	Var x, y, xo, yo, xi, yi;
	ImageParam input;
	RDom r;
	Func prepare, blur_x, blur_y, offload;

    HalidePipeline() :
        input(type_of<float>(), 2),
        x("x"), y("y"), xo("xo"), yo("yo"), xi("xi"), yi("yi"),
        r(0, 9),
        prepare("prepare"), blur_x("blur_x"), blur_y("blur_y"), offload("offload")
    {
        prepare = BoundaryConditions::repeat_edge(input);
	    blur_x(x, y) = sum(prepare(x + r.x, y)) / 9;
        offload(x, y) = sum(blur_x(x, y + r.x)) / 9;
        blur_y(x, y) = offload(x, y);
    }


    void compile_to_hls() {
	    blur_y.tile(x, y, xo, yo, xi, yi, 480, 360);
        prepare.compute_at(blur_y, xo);
        offload.compute_at(blur_y, xo);
	    offload.tile(x, y, xo, yo, xi, yi, 480, 360);
        offload.offload({blur_x}, xo);
	    blur_y.compile_to_lowered_stmt("ir.html", {input}, HTML);
    }

    void compile_to_cpu() {
	    blur_y.tile(x, y, xo, yo, xi, yi, 480, 360);
        blur_x.compute_at(blur_y, xo);
	    blur_y.compile_to_lowered_stmt("ir.html", {input}, HTML);
        blur_y.compile_to_file("blur", {input}, "blur");
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
        HalidePipeline().compile_to_hls();
    } else {
        std::cerr << "Unknown target!\n";
        return 1;
    }
	
	std::cout << "Generated\n";
	return 0;
}

