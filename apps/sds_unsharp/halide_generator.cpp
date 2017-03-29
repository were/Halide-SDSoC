#include <iostream>
#include "Halide.h"


using namespace Halide;

struct HalidePipeline {
    ImageParam input;
    Var x, y, c, xi, yi, xo, yo;
    RDom filter;
    Func /*r, g, b, */prepare, blur, gray, sharpen, ratio, offload, res;
    Func kernel_f, kernel;
    std::vector<ImageParam> param;

    HalidePipeline() 
        : x("x"), y("y"), c("c"), xi("xi"), yi("yi"), xo("xo"), yo("yo"), filter(-4, 9, -4, 9),
        input(UInt(8), 3),
        //r("r"), g("g"), b("b"),
        prepare("prepare"),
        blur("blur"), gray("gray"), sharpen("sharpen"), ratio("ratio"), offload("offload"), res("res"),
        kernel_f("kernel_f"), kernel("kernel"),
        param{input} {
        float sigma = 1.5;

        //constants difinition
        kernel_f(x) = exp(-x * x / (2 * sigma * sigma)) / sqrtf(2 * M_PI * sigma);

        kernel(x) = cast<uint8_t> (
                kernel_f(x) * 255 / (kernel_f(0) + kernel_f(1) * 2 + kernel_f(2) * 2 + kernel_f(3) * 2 + kernel_f(4) * 2)
        );

        //software side data preparation
        prepare(c, x, y) = input(x + 4, y + 4, c);
        /*r(x, y) = input(x + 4, y + 4, 0);
        g(x, y) = input(x + 4, y + 4, 1);
        b(x, y) = input(x + 4, y + 4, 2);*/

        gray(x, y) = cast<uint8_t>(
                (cast<uint16_t>(prepare(0, x, y)) * 77 +
                 cast<uint16_t>(prepare(1, x, y)) * 150 +
                 cast<uint16_t>(prepare(2, x, y)) * 29) >> 8);

        blur(x, y) = cast<uint8_t>(sum(cast<uint32_t>(gray(filter.x + x, filter.y + y)) * kernel(filter.x) * kernel(filter.y)) >> 16);
        /*blur(x, y) = 0;
        blur(x, y) += cast<uint32_t>(gray(filter.x + x, filter.y + y)) * kernel(filter.x) * kernel(filter.y);
        blur(x, y) = blur(x, y) >> 16;*/

        sharpen(x, y) = cast<uint8_t>(clamp(cast<uint16_t>(gray(x, y) * 2) - blur(x, y), 0, 255));

        ratio(x, y) = cast<uint8_t>(clamp(cast<uint16_t>(sharpen(x, y)) * 32 / max(gray(x, y), 1), 0, 255));

        offload(c, x, y) = cast<uint8_t>(clamp(cast<uint16_t>(ratio(x, y)) * 
                    prepare(c, x, y) >> 5, 0, 255));

        res(x, y, c) = offload(c, x, y);

        //res.bound(c, 0, 3);
        offload.bound(c, 0, 3);

        std::cerr << "Algorithm defined...\n";
    }

    void compile_to_cpu() {
	    res.compile_to_lowered_stmt("ir.cpu.html", {input}, HTML);
        res.compile_to_file("unsharp", {input}, "unsharp");
    }

    void compile_to_hls() {
        res.tile(x, y, xo, yo, xi, yi, 480, 640);
        offload.tile(x, y, xo, yo, xi, yi, 480, 640);
        prepare.compute_at(res, xo);
        offload.compute_at(res, xo);
        offload.offload({gray, ratio}, xo);
        offload.unroll(c);
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

