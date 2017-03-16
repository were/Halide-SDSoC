#ifndef BUFFER_H
#define BUFFER_H

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

    T &operator() (int x, int y) {
        return ((T*) content->host)[(x - content->min[0]) * content->stride[0] + (y - content->min[1]) * content->stride[1]];
    }

    const T &operator() (int x, int y) const {
        return ((const T*) content->host)[(x - content->min[0]) * content->stride[0] + (y - content->min[1]) * content->stride[1]];
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
