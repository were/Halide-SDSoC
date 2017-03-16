#include <cassert>
#ifndef HLS_VIDEO_MEM_H
#define HLS_VIDEO_MEM_H

namespace hls {

    template<int rows, int columns, typename T>
    class LineBuffer {
        T val[rows][columns];

    public:
        void shift_up(int col) {
            shift_pixels_up(col);
        }
        void shift_pixels_up(int col) {
            assert(col >= 0 && col < columns);
            for (size_t i = 1; i < rows; ++i) {
                val[i - 1][col] = val[i][col];
            }
        }
        void insert_top(T value, int col) {
            insert_bottom_row(value, col);
        }
        void insert_bottom_row(T value, int col) {
            //std::cerr << col << "\n";
            assert(col >= 0 && col < columns);
            val[rows - 1][col] = value;
        }
        T& operator() (int x, int y) {
            assert(x >= 0 && x < rows && y >= 0 && y < columns);
            return val[x][y];
        }

    };

    template<int rows, int columns, typename T>
    class Window {
        T val[rows][columns];
    public:
        void shift_left() {
            shift_pixels_left();
        }
        void shift_pixels_left() {
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 1; j < columns; ++j) {
                    val[i][j - 1] = val[i][j];
                }
            }
        }
        T& operator() (int x, int y) {
            assert(x >= 0 && x < rows && y >= 0 && y < columns);
            return val[x][y];
        }
    };

}

#endif
