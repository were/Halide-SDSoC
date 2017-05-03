#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <ctime>

#include "cpu.h"
#include "top.h"
#include "Test.h"

//using namespace Halide::Runtime;

#define WIDTH 480
#define HEIGHT 640

int main(int argc, char **argv) {

    Buffer<uint8_t> input(WIDTH, HEIGHT);
    Buffer<uint8_t> answer(WIDTH, HEIGHT);
    Buffer<uint8_t> output(WIDTH, HEIGHT);

    test_equiv(input, answer, output, cpu, top);

    return 0;
}
