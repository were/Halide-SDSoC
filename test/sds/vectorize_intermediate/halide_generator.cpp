#include <iostream>
#include "Halide.h"


using namespace Halide;

struct HalidePipeline {
    ImageParam input;
    Var x, y, c, xi, yi, xo, yo;
    Func prepare, blur_x, blur_y, offload, res;
    RDom filter;
    std::vector<ImageParam> param;

    HalidePipeline() 
        : x("x"), y("y"), c("c"), xi("xi"), yi("yi"), xo("xo"), yo("yo"), blur_x("blur_x"), filter(-1, 3),
        input(UInt(8), 3), prepare("prepare"), offload("offload"), res("res"),
        param{input} {


        //software side data preparation
        prepare = BoundaryConditions::repeat_edge(input);

        blur_x(c, x, y) = cast<uint8_t>(sum(cast<uint32_t>(prepare(x + filter.x, y, c))) / 3);

        offload(c, x, y) = cast<uint8_t>(sum(cast<uint32_t>(blur_x(c, x, y + filter.x))) / 3);

        offload.bound(c, 0, 3);

        res(x, y, c) = offload(c, x, y);

        std::cerr << "Algorithm defined...\n";
    }

    void compile_to_cpu() {
	    //res.compile_to_lowered_stmt("ir.cpu.html", {input}, HTML);
        res.compile_to_c("cpu.cpp", {input}, "cpu");
        res.compile_to_header("cpu.h", {input}, "cpu");
    }

    void compile_to_hls() {
        res.tile(x, y, xo, yo, xi, yi, 480, 640);
        offload.tile(x, y, xo, yo, xi, yi, 480, 640);
        prepare.compute_at(res, xo);
        offload.compute_at(res, xo);
        offload.offload({blur_x.unroll(c)}, xo).unroll(c);
        //offload.unroll(c);
	    //res.compile_to_lowered_stmt("ir.hls.html", {input}, HTML);
        res.compile_to_sdsoc("top", {input}, "top");
        std::cerr << "Compiled...\n";
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
    }
	return 0;
}

