#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include "Test.h"
#include "top.h"
#include "cpu.h"

int main(int argc, char **argv) {

    Buffer<uint8_t> input(128, 128, 3);
    Buffer<uint8_t> weight(5, 5, 3);
    Buffer<uint32_t> answer(128, 128, 3);
    Buffer<uint32_t> output(128, 128, 3);

    test_equiv(input, weight, answer, output, cpu, top);

    return 0;
}
