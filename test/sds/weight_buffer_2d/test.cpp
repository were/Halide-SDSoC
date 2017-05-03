#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include "Test.h"
#include "top.h"
#include "cpu.h"

int main(int argc, char **argv) {

    Buffer<uint8_t> input(512, 512);
    Buffer<uint8_t> weight(5, 5);
    Buffer<int> answer(512, 512);
    Buffer<int> output(512, 512);

    test_equiv(input, weight, answer, output, cpu, top);

    return 0;
}
