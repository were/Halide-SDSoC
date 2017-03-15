#include "hls_video.h"
#include "hls_stream.h"
#include <iostream>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
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
struct halide_filter_metadata_t;
#ifdef _WIN32
float roundf(float);
double round(double);
#else
inline float asinh_f32(float x) {return asinhf(x);}
inline float acosh_f32(float x) {return acoshf(x);}
inline float atanh_f32(float x) {return atanhf(x);}
inline double asinh_f64(double x) {return asinh(x);}
inline double acosh_f64(double x) {return acosh(x);}
inline double atanh_f64(double x) {return atanh(x);}
#endif
inline float sqrt_f32(float x) {return sqrtf(x);}
inline float sin_f32(float x) {return sinf(x);}
inline float asin_f32(float x) {return asinf(x);}
inline float cos_f32(float x) {return cosf(x);}
inline float acos_f32(float x) {return acosf(x);}
inline float tan_f32(float x) {return tanf(x);}
inline float atan_f32(float x) {return atanf(x);}
inline float sinh_f32(float x) {return sinhf(x);}
inline float cosh_f32(float x) {return coshf(x);}
inline float tanh_f32(float x) {return tanhf(x);}
inline float hypot_f32(float x, float y) {return hypotf(x, y);}
inline float exp_f32(float x) {return expf(x);}
inline float log_f32(float x) {return logf(x);}
inline float pow_f32(float x, float y) {return powf(x, y);}
inline float floor_f32(float x) {return floorf(x);}
inline float ceil_f32(float x) {return ceilf(x);}
inline float round_f32(float x) {return roundf(x);}

inline double sqrt_f64(double x) {return sqrt(x);}
inline double sin_f64(double x) {return sin(x);}
inline double asin_f64(double x) {return asin(x);}
inline double cos_f64(double x) {return cos(x);}
inline double acos_f64(double x) {return acos(x);}
inline double tan_f64(double x) {return tan(x);}
inline double atan_f64(double x) {return atan(x);}
inline double sinh_f64(double x) {return sinh(x);}
inline double cosh_f64(double x) {return cosh(x);}
inline double tanh_f64(double x) {return tanh(x);}
inline double hypot_f64(double x, double y) {return hypot(x, y);}
inline double exp_f64(double x) {return exp(x);}
inline double log_f64(double x) {return log(x);}
inline double pow_f64(double x, double y) {return pow(x, y);}
inline double floor_f64(double x) {return floor(x);}
inline double ceil_f64(double x) {return ceil(x);}
inline double round_f64(double x) {return round(x);}

inline float nan_f32() {return NAN;}
inline float neg_inf_f32() {return -INFINITY;}
inline float inf_f32() {return INFINITY;}
inline bool is_nan_f32(float x) {return x != x;}
inline bool is_nan_f64(double x) {return x != x;}
template<typename A, typename B> A reinterpret(B b) {A a; memcpy(&a, &b, sizeof(a)); return a;}
inline float float_from_bits(uint32_t bits) {return reinterpret<float, uint32_t>(bits);}

template<typename T> T max(T a, T b) {if (a > b) return a; return b;}
template<typename T> T min(T a, T b) {if (a < b) return a; return b;}

inline uint8_t *_halide_buffer_get_host(const buffer_t *buf) {
 return buf->host;
}
inline int _halide_buffer_get_min(const buffer_t *buf, int d) {
 return buf->min[d];
}
inline int _halide_buffer_get_max(const buffer_t *buf, int d) {
 return buf->min[d] + buf->extent[d] - 1;
}
inline int _halide_buffer_set_dev_dirty(buffer_t *buf, bool val) {
 buf->dev_dirty = val;
 return 0;
}
inline int _halide_buffer_set_host_dirty(buffer_t *buf, bool val) {
 buf->host_dirty = val;
 return 0;
}
inline buffer_t *_halide_buffer_init(buffer_t *dst, void *host, uint64_t dev, int /*type_code*/, int type_bits, int dimensions, 
                                     const void *min, const void *extent, const void *stride, bool host_dirty, bool dev_dirty) {
 dst->host = (uint8_t *)host;
 dst->dev = dev;
 dst->elem_size = (type_bits + 7) / 8;
 dst->host_dirty = host_dirty;
 dst->dev_dirty = dev_dirty;
 for (int i = 0; i < dimensions; i++) {
  dst->min[i] = ((const int *)min)[i];
  dst->extent[i] = ((const int *)extent)[i];
  dst->stride[i] = ((const int *)stride)[i];
 }
 for (int i = dimensions; i < 4; i++) {
  dst->min[i] = 0;
  dst->extent[i] = 0;
  dst->stride[i] = 0;
 }
 return dst;
}

#ifndef HALIDE_FUNCTION_ATTRS
#define HALIDE_FUNCTION_ATTRS
#endif
void offload(
 uint8_t _p0[100 * 100],
 uint8_t _offload[100 * 100]) {
 #pragma HLS dataflow
 hls::stream<uint8_t> _p0_to_offload_s0_stream;
 (void)0;
_offload_s0_x_xi:
  for (int _offload_s0_x_xi = 0; _offload_s0_x_xi < 100; _offload_s0_x_xi++)
  {
  #pragma HLS pipeline II=1
  #pragma HLS loop_flatten off
    /*uint8_t _p0_holder[1];
    #pragma HLS array_partition variable=_p0_holder complete dim=0
    uint8_t _271 = _p0_to_offload_s0_stream.read();
    _p0_holder[0] = _271;
    int32_t _272 = _offload_s0_y_yi * 100;
    int32_t _273 = _offload_s0_x_xi + _272;
    uint8_t _274 = _p0_holder[0];*/
    int32_t _273 = _offload_s0_x_xi;
    uint8_t _274 = 0;
    _offload[_273] = _274;
  } // for _offload_s0_x_xi
 //_offload[0] = 1;
}
