#include <iostream>
#include "Halide.h"


using namespace Halide;

struct HalidePipeline {
    ImageParam input;
    Var x, y, xi, yi, xo, yo;
    RDom filter99, filter33;
    Func prepare, lighten, darken, blur33, blur99, offload, res;
    std::vector<ImageParam> param;

    HalidePipeline() 
        : x("x"), y("y"), xi("xi"), yi("yi"), xo("xo"), yo("yo"),
        filter99(-4, 9, -4, 9), filter33(-1, 3, -1, 3),
        input(UInt(8), 2),
        prepare("prepare"), lighten("lighten"), darken("darken"),
        blur33("blur33"), blur99("blur99"), offload("offload"), res("res"),
        param{input} {

        //software side data preparation
        prepare = BoundaryConditions::repeat_edge(input);
        //prepare(x, y) = input(x + 4, y + 4);

        lighten(x, y) = 
            cast<uint8_t>(clamp(cast<uint16_t>(prepare(x, y)) * 12 / 10, 0, 255));

        darken(x, y) = 
            cast<uint8_t>(clamp(cast<uint16_t>(prepare(x, y)) * 10 / 12, 0, 255));

        blur33(x, y) = 
            cast<uint8_t>((sum(cast<uint32_t>(lighten(filter33.x + x, filter33.y + y)))) / 9);

        blur99(x, y) = 
            cast<uint8_t>((sum(cast<uint32_t>(darken(filter99.x + x, filter99.y + y)))) / 81);

        offload(x, y) =
            cast<uint8_t>(clamp(cast<uint16_t>(prepare(x, y)) * cast<uint16_t>(blur33(x, y)) / cast<uint16_t>(blur99(x, y)), 0, 255));

        res(x, y) = offload(x, y);

        std::cerr << "Algorithm defined...\n";
    }

    void compile_to_cpu() {
        /*res.tile(x, y, xo, yo, xi, yi, 5, 5);
        offload.tile(x, y, xo, yo, xi, yi, 5, 5);
        prepare.compute_at(res, xo);
        offload.compute_at(res, xo);
        //offload.offload({lighten, darken}, xo);
        lighten.compute_at(offload, xo);
        darken.compute_at(offload, xo);*/
	    res.compile_to_lowered_stmt("ir.cpu.html", {input}, HTML);
        res.compile_to_c("cpu.cpp", {input}, "cpu");
        res.compile_to_header("cpu.h", {input}, "cpu");
        std::cerr << "Compiled...\n";
    }

    void compile_to_hls() {
        res.tile(x, y, xo, yo, xi, yi, 480, 640);
        offload.tile(x, y, xo, yo, xi, yi, 480, 640);
        prepare.compute_at(res, xo);
        offload.compute_at(res, xo);

        offload.offload({lighten, darken}, xo);
        offload.stream_depth(prepare, 6 * 640 + 1);

        //offload.offload({blur33, blur99}, xo);
        
	    res.compile_to_lowered_stmt("ir.hls.html", {input}, HTML);
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

