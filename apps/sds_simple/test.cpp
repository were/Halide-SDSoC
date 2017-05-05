#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include "Buffer.h"
#include "top.h"
#include "cpu.h"

int main(int argc, char **argv) {

    Buffer<uint32_t> input(256);
    Buffer<uint32_t> answer(256 - 3 + 1);
    Buffer<uint32_t> output(256 - 3 + 1);

    for (int x = 0; x < input.width(); x++) {
        input(x) = rand() & 0xfff;
    }

    std::cerr << "Data prepare done!\n";

    time_t start = clock();
    cpu(input, answer);

    std::cerr << "CPU code done!\n";
    time_t interval(clock() - start);
    std::cerr << "Time: " << interval / CLOCKS_PER_SEC << "." << interval % CLOCKS_PER_SEC << "\n";

    start = clock();
    top(input, output);

    std::cerr << "FPGA CSIM code done!\n";
    interval = clock() - start;
    std::cerr << "Time: " << interval / CLOCKS_PER_SEC << "." << interval % CLOCKS_PER_SEC << "\n";

    for (int x = 0; x < output.width(); x++)
        if (answer(x) != output(x)) {
            std::cerr << "Error @" << x << "\n";
            std::cerr << answer(x) << " != " << output(x) << "\n";
            return 1;
        }

    std::cerr << "OK!\n";
    return 0;
}
