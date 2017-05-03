#ifndef TEST_H
#define TEST_H

#include "Buffer.h"
#include <iostream>

using std::cerr;

typedef int (*func11)(buffer_t *a, buffer_t *b);
typedef int (*func21)(buffer_t *a, buffer_t *b, buffer_t *c);

template<typename T>
bool compare(Buffer<T> a, Buffer<T> b) {
    assert(a.dims() == b.dims());
    assert(a.width() == b.width());
    if (a.dims() == 1) {
        for (int x = 0; x < a.width(); ++x) {
            if (a(x) != b(x)) {
                cerr << "Buffer diff @" << x << "\n";
                return false;
            }
        }
    } else {
        assert(a.height() == b.height());
        if (a.dims() == 2) {
            for (int y = 0; y < a.height(); ++y) {
                for (int x = 0; x < a.width(); ++x) {
                    if (a(x, y) != b(x, y)) {
                        cerr << "Buffer diff @" << x << ", " << y << "\n";
                        return false;
                    }
                }
            }
        } else {
            assert(a.channels() == b.channels());
            if (a.dims() == 3) {
                for (int c = 0; c < a.channels(); ++c) {
                    for (int y = 0; y < a.height(); ++y) {
                        for (int x = 0; x < a.width(); ++x) {
                            if (a(x, y, c) != b(x, y, c)) {
                                cerr << "Buffer diff @" << x << ", " << y << ", " << c << "\n";
                                return false;
                            }
                        }
                    }
                }
            } else {
                assert(false);
            }
        }
    }
    return true;
}

template<typename T0, typename T1>
void test_equiv(Buffer<T0> input, Buffer<T1> answer, Buffer<T1> output, func11 cpu, func11 sds) {
    input.random();

	time_t start = clock();
    cpu(input, answer);

    std::cerr << "CPU code done!\n";
	std::cerr << "Time: " << (double) (clock() - start) / CLOCKS_PER_SEC << "\n";

	start = clock();
    sds(input, output);

    std::cerr << "FPGA CSIM code done!\n";
	std::cerr << "Time: " << (double) (clock() - start) / CLOCKS_PER_SEC << "\n";

    if (compare(output, answer)) {
        std::cerr << "OK! Function equiv test passed!\n";
    } else {
        std::cerr << "Something wrong makes the functionality different, sorry!\n";
    }
}

template<typename T0, typename T1, typename T2>
void test_equiv(Buffer<T0> input, Buffer<T1> weight, Buffer<T2> answer, Buffer<T2> output, func21 cpu, func21 sds) {
    input.random();
    weight.random();

	time_t start = clock();
    cpu(input, weight, answer);

    std::cerr << "CPU code done!\n";
	std::cerr << "Time: " << (double) (clock() - start) / CLOCKS_PER_SEC << "\n";

	start = clock();
    sds(input, weight, output);

    std::cerr << "FPGA CSIM code done!\n";
	std::cerr << "Time: " << (double) (clock() - start) / CLOCKS_PER_SEC << "\n";

    if (compare(output, answer)) {
        std::cerr << "OK! Function equiv test passed!\n";
    } else {
        std::cerr << "Something wrong makes the functionality different, sorry!\n";
    }
}

#endif
