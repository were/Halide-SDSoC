#include <iostream>
#include "Halide.h"


using namespace Halide;

struct HalidePipeline {
    ImageParam input;
    Var x, y, xi, yi, xo, yo;
    Func prepare, gray, offload, res;
    std::vector<ImageParam> param;

    HalidePipeline() 
        : x("x"), y("y"), xi("xi"), yi("yi"), xo("xo"), yo("yo"),
        input(UInt(8), 3), prepare("prepare"),
        gray("gray"), offload("offload"), res("res"),
        param{input} {


        //software side data preparation
        prepare = input;

        offload(x, y) = cast<uint8_t>(
                (cast<uint16_t>(prepare(x, y, 0)) * 77 +
                 cast<uint16_t>(prepare(x, y, 1)) * 150 +
                 cast<uint16_t>(prepare(x, y, 2)) * 29) >> 8);


        res(x, y) = offload(x, y);

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
        offload.offload({}, xo);
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
