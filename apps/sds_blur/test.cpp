#include <emmintrin.h>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>

#include "benchmark.h"
#include "HalideBuffer.h"
#include "blur.h"

using namespace Halide::Runtime;
using std::clog;
using std::cerr;

const int width = 960, height = 720;
const float EPS = 1e-2;

int main(int argc, char **argv) {

    Buffer<float> input(width, height),
        out_ref(width - 8, height - 8),
        out_obj(width - 8, height - 8);

    for (int y = 0; y < input.height(); y++) {
        for (int x = 0; x < input.width(); x++) {
            input(x, y) = (float) (rand() & 0xfff);
        }
    }

    clog << "Input init done...\n";

    for (int y = 0; y < input.height() - 8; y++) for (int x = 0; x < input.width() - 8; x++) {
        out_ref(x, y) = 0;
        for (int dy = 0; dy < 9; ++dy) for (int dx = 0; dx < 9; ++dx)
            out_ref(x, y) += input(x + dx, y + dy);
        out_ref(x, y) *= 0.111111;
        out_ref(x, y) *= 0.111111;
    }

    clog << "Manual C done...\n";

    blur(input, out_obj);

    clog << "Halide obj done...\n";

    for (int y = 0; y < input.height() - 8; y++) for (int x = 0; x < input.width() - 8; x++) {
        if (std::abs(out_ref(x, y) - out_obj(x, y)) > EPS) {
            std::cerr << "Error @(" << x << ", " << y << ")\n";
            std::cerr << out_ref(x, y) << "\n";
            std::cerr << out_obj(x, y) << "\n";
            std::cerr << out_ref(x, y) - out_obj(x, y) << "\n";
            return 1;
        }
    }

    std::clog << "OK!\n";

    return 0;
}
