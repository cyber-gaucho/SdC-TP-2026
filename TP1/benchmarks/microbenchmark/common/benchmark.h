// common/benchmark.h

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdint.h>

#define ITERATIONS 200000000

volatile uint32_t int_result;
volatile float float_result;

static inline void test_integers() {
    uint32_t acc = 0;
    for (uint32_t i = 0; i < ITERATIONS; i++) {
        acc += i;
        acc ^= (acc << 1);
    }
    int_result = acc;
}

static inline void test_floats() {
    float acc = 0.0f;
    for (uint32_t i = 0; i < ITERATIONS; i++) {
        acc += (float)i * 0.000001f;
        acc *= 1.000001f;
    }
    float_result = acc;
}

#endif