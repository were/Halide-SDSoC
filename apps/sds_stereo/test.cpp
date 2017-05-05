#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <ctime>

#include "cpu.h"
#include "top.h"
#include "Buffer.h"

#define WIDTH 480
#define HEIGHT 640

//using namespace Halide::Runtime;

int main(int argc, char **argv) {

    Buffer<uint8_t> input(WIDTH, HEIGHT, 3);
    Buffer<uint8_t> answer(WIDTH, HEIGHT, 3);
    Buffer<uint8_t> output(WIDTH, HEIGHT, 3);

    for (int z = 0; z < input.channels(); ++z)
    for (int y = 0; y < input.height(); y++)
    for (int x = 0; x < input.width(); x++)
            input(x, y, z) = rand() & 0xfff;

    std::cerr << "Data prepare done!\n";

    time_t start = clock();
    cpu(input, input, input, input, answer);

    std::cerr << "CPU code done!\n";
    std::cerr << "Time: " << clock() - start << "\n";

    start = clock();
    top(input, input, input, input, output);

    std::cerr << "FPGA CSIM code done!\n";
    std::cerr << "Time: " << clock() - start << "\n";
/*
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
*/
    for (int z = 0; z < input.channels(); ++z)
    for (int y = 0; y < input.height(); y++) {
        for (int x = 0; x < input.width(); x++)
            if (answer(x, y, z) != output(x, y, z)) {
                std::cerr << "Error @" << x << ", " << y << "\n";
                std::cerr << answer(x, y) << " != " << output(x, y) << "\n";
                return 1;
            }
    }

    std::cerr << "OK!\n";
    return 0;
}
