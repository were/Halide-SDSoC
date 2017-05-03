#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <ctime>

#include "cpu.h"
#include "top.h"
#include "Test.h"

#define WIDTH 480
#define HEIGHT 640

//using namespace Halide::Runtime;

int main(int argc, char **argv) {

    Buffer<uint8_t> input(WIDTH, HEIGHT, 3);
    Buffer<uint8_t> answer(WIDTH, HEIGHT);
    Buffer<uint8_t> output(WIDTH, HEIGHT);
    test_equiv(input, answer, output, cpu, top);

    return 0;
}
