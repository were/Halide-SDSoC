#ifndef BUFFER_H
#define BUFFER_H

#include <assert.h>

#ifndef HALIDE_ATTRIBUTE_ALIGN
  #ifdef _MSC_VER
    #define HALIDE_ATTRIBUTE_ALIGN(x) __declspec(align(x))
  #else
    #define HALIDE_ATTRIBUTE_ALIGN(x) __attribute__((aligned(x)))
  #endif
#endif

#ifndef BUFFER_T_DEFINED
#define BUFFER_T_DEFINED
#include <stdbool.h>
#include <stdint.h>
typedef struct buffer_t {
    uint64_t dev;
    uint8_t* host;
    int32_t extent[4];
    int32_t stride[4];
    int32_t min[4];
    int32_t elem_size;
    HALIDE_ATTRIBUTE_ALIGN(1) bool host_dirty;
    HALIDE_ATTRIBUTE_ALIGN(1) bool dev_dirty;
    HALIDE_ATTRIBUTE_ALIGN(1) uint8_t _padding[10 - sizeof(void *)];
} buffer_t;
#endif

template<typename T>
struct Buffer {
    buffer_t *content;
    Buffer(int width) {
        content = new buffer_t;
        content->dev = 0;
        content->host = (uint8_t*) (new T[width]);
        content->extent[0] = width;
        content->stride[0] = 1;
        content->extent[1] = 0;
        content->stride[1] = 0;
        content->extent[2] = 0;
        content->stride[2] = 0;
        content->extent[3] = 0;
        content->stride[3] = 0;
        content->min[0]= 0;
        content->min[1]= 0;
        content->min[2]= 0;
        content->min[3]= 0;
        content->elem_size = sizeof (T);
    }

    Buffer(int width, int height) {
        content = new buffer_t;
        content->dev = 0;
        content->host = (uint8_t*) (new T[width * height]);
        content->extent[0] = width;
        content->stride[0] = 1;
        content->extent[1] = height;
        content->stride[1] = width;
        content->extent[2] = 0;
        content->stride[2] = 0;
        content->extent[3] = 0;
        content->stride[3] = 0;
        content->min[0]= 0;
        content->min[1]= 0;
        content->min[2]= 0;
        content->min[3]= 0;
        content->elem_size = sizeof (T);
    }

    Buffer(int width, int height, int channels) {
        content = new buffer_t;
        content->dev = 0;
        content->host = (uint8_t*) (new T[width * height * channels]);
        content->extent[0] = width;
        content->stride[0] = 1;
        content->extent[1] = height;
        content->stride[1] = width;
        content->extent[2] = channels;
        content->stride[2] = width * height;
        content->extent[3] = 0;
        content->stride[3] = 0;
        content->min[0]= 0;
        content->min[1]= 0;
        content->min[2]= 0;
        content->min[3]= 0;
        content->elem_size = sizeof (T);
    }

    T &operator() (int w, int x = 0, int y = 0, int z = 0) {
        return ((T*) content->host)[
            (w - content->min[0]) * content->stride[0] + 
            (x - content->min[1]) * content->stride[1] + 
            (y - content->min[2]) * content->stride[2] + 
            (z - content->min[3]) * content->stride[3]
        ];
    }

    const T &operator() (int w, int x = 0, int y = 0, int z = 0) const {
        return ((const T*) content->host)[
            (w - content->min[0]) * content->stride[0] + 
            (x - content->min[1]) * content->stride[1] + 
            (y - content->min[2]) * content->stride[2] + 
            (z - content->min[3]) * content->stride[3]
        ];
    }

    int dims() {
        for (int i = 0; i < 4; ++i) {
            if (content->extent[i] == 0) {
                return i;
            }
        }
        assert(false);
        return -1;
    }

    void random() {
        int _dims = dims();
        int size = 1;
        for (int i = 0; i < _dims; ++i) {
            size *= content->extent[i];
        }
        for (int i = 0; i < size; ++i) {
            if (content->elem_size == 1) {
                ((T*) content->host)[i] = (uint8_t) (rand() & 255);
            } else if (content->elem_size == 2) {
                ((T*) content->host)[i] = (uint16_t) (rand() & 32767);
            } else if (content->elem_size == 4) {
                ((T*) content->host)[i] = rand() | (rand() % 2 << 31);
            } else if (content->elem_size == 8) {
                ((T*) content->host)[i] = ((uint64_t) rand() << 32) | rand() | (rand() % 2 << 31) | ((uint64_t) (rand() % 2) << 63);
            } else {
                assert(false);
            }
        }
    }

    int channels() {
        return (int) content->extent[2];
    }

    int height() {
        return (int) content->extent[1];
    }

    int width() {
        return (int) content->extent[0];
    }

    operator buffer_t * () {
        return content;
    }
};

#endif
