#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include "Buffer.h"
#include "top.h"
#include "cpu.h"

int main(int argc, char **argv) {

    Buffer<uint8_t> input(512, 512);
    Buffer<uint8_t> weight(5, 5);
    Buffer<uint32_t> answer(512, 512);
    Buffer<uint32_t> output(512, 512);

    for (int y = 0; y < input.height(); y++) {
        for (int x = 0; x < input.width(); x++) {
            input(x, y) = rand() & 0xfff;
        }
    }

    for (int y = 0; y < weight.height(); y++) {
        for (int x = 0; x < weight.width(); x++) {
            weight(x, y) = rand() & 0xfff;
        }
    }

    std::cerr << "Data prepare done!\n";

    time_t start = clock();
    cpu(input, weight, answer);

    std::cerr << "CPU code done!\n";
    std::cerr << "Time: " << clock() - start << "\n";

    start = clock();
    top(input, weight, output);

    std::cerr << "FPGA CSIM code done!\n";
    std::cerr << "Time: " << clock() - start << "\n";
/*
    for (int y = 0; y < input.height(); y++) {
        for (int x = 0; x < input.width(); x++)
            std::cerr << (int) input(x, y) << " ";
        std::cerr << "\n";
    }
    std::cerr << "\n";

    for (int y = 0; y < weight.height(); y++) {
        for (int x = 0; x < weight.width(); x++)
            std::cerr << (int) weight(x, y) << " ";
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
