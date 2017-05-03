#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include "top.h"
#include "cpu.h"
#include "Test.h"

int main(int argc, char **argv) {

    Buffer<uint32_t> input(128);
    Buffer<uint32_t> answer(128);
    Buffer<uint32_t> output(128);

    test_equiv(input, answer, output, cpu, top);

    return 0;
}
