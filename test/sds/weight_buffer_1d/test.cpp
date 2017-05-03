#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include "Test.h"
#include "top.h"
#include "cpu.h"

int main(int argc, char **argv) {

    Buffer<uint8_t> input(512);
    Buffer<uint8_t> weight(5);
    Buffer<uint32_t> answer(512);
    Buffer<uint32_t> output(512);

    test_equiv(input, weight, answer, output, cpu, top);

    return 0;
}
