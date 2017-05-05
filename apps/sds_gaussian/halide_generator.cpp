#include <iostream>
#include "Halide.h"


using namespace Halide;

struct HalidePipeline {
    ImageParam input;
    Var x, y, xi, yi, xo, yo;
    RDom filter99, filter33;
    Func kernel, kernel_f, prepare, lighten, offload, res;
    std::vector<ImageParam> param;

    HalidePipeline() 
        : x("x"), y("y"), xi("xi"), yi("yi"), xo("xo"), yo("yo"),
        filter33(-4, 9, -4, 9),
        input(UInt(8), 2),
        prepare("prepare"), offload("offload"), res("res"),
        param{input} {

        float sigma = 1.5f;
        kernel_f(x) = exp(-x*x/(2*sigma*sigma)) / (sqrtf(2*M_PI)*sigma);
        // normalize and convert to 8bit fixed point
        kernel(x) = cast<uint8_t>(kernel_f(x) * 255 / (kernel_f(0) + kernel_f(1)*2 + kernel_f(2)*2 + kernel_f(3)*2 + kernel_f(4)*2));


        //software side data preparation
        prepare = BoundaryConditions::repeat_edge(input);
        //prepare(x, y) = input(x + 4, y + 4);

        offload(x, y) = 
            cast<uint8_t>((sum(cast<uint32_t>(prepare(filter33.x + x, filter33.y + y)) * kernel(filter33.x) * kernel(filter33.y)))
                    >> 16);

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

        offload.offload({}, xo);
        
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

