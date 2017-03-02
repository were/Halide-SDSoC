#include <emmintrin.h>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdio>

#include "cpu.h"
#include "top.h"
#include "HalideBuffer.h"

using namespace Halide::Runtime;

int main(int argc, char **argv) {

    Buffer<uint8_t> input(200, 200);
    Buffer<uint8_t> answer(200, 200);
    Buffer<uint8_t> output(200, 200);

    for (int y = 0; y < input.height(); y++) {
        for (int x = 0; x < input.width(); x++) {
            input(x, y) = rand() & 0xfff;
        }
    }

    std::cerr << "Data prepare done!\n";

    cpu(input, answer);

    std::cerr << "CPU code done!\n";

    top(input, output);

    std::cerr << "FPGA CSIM code done!\n";

    for (int y = 0; y < input.height(); y++) {
        for (int x = 0; x < input.width(); x++)
            std::cerr << (int) input(x, y) << " ";
        std::cerr << "\n";
    }
    std::cerr << "\n";

    for (int y = 0; y < input.height(); y++) {
        for (int x = 0; x < input.width(); x++)
            std::cerr << (int) answer(x, y) << " ";
        std::cerr << "\n";
    }
    std::cerr << "\n";

    for (int y = 0; y < input.height(); y++) {
        for (int x = 0; x < input.width(); x++)
            std::cerr << (int) output(x, y) << " ";
        std::cerr << "\n";
    }
    std::cerr << "\n";

    for (int y = 0; y < input.height(); y++) {
        for (int x = 0; x < input.width(); x++)
            if (answer(x, y) != output(x, y)) {
                std::cerr << "Error @" << x << ", " << y << "\n";
                std::cerr << answer(x, y) << " != " << output(x, y) << "\n";
                return 1;
            }
    }

    std::cerr << "OK!\n";
    return 0;
}
