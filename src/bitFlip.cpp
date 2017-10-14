/*
 * The MIT License
 *
 * Copyright 2017 Anthony Campbell.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
/**
 * @author Anthony Campbell contact@claydonkey.com
 */
/*
 * File:   bitFlip.cpp
 * Author: Anthony Campbell
 *
 * Created on 05 October 2017, 11:13
 */

#include <math.h>
#include <omp.h>
#include "bitFlip.h"
#include "BitReverseTable16.h"
#include <immintrin.h>

#ifdef __cplusplus

#ifdef NOASSMBLR

void bitflipbyte(uint8_t[], uint32_t, uint8_t[]) {
}

void bitflipllloop(uint64_t * n, uint32_t size) {
}
#endif

void bitFlipNaiveLambda(uint64_t * bits, uint32_t size) {

    uint64_t r = 0;
    uint8_t c = 0;
    const uint8_t s = sizeof (uint64_t)*8;
    for (uint32_t i = 0; i < size; i++) bits[i] = [&] (uint64_t i) -> uint64_t {
            r = 0;
            for (c = 0; c < s; c++) r |= ((i >> c) & 1) << (s - c - 1);
            return r;
        }(bits[i]);

}

extern "C" {

    uint16_t bitFlipNaives(const uint16_t bits) {
        return bitFlipNaive<uint16_t> (bits);
    }

    uint64_t bitFlipNaivell(const uint64_t bits) {
        return bitFlipNaive<uint64_t> (bits);
    }

    __attribute__ ((aligned(32))) uint8_t c[1 + 32] = {};

    uint8_t bitFlipZb(uint8_t bits) {
        uint8_t r = 0;
        c[0] = bits;
        bitFlipZ<uint8_t> (c);
        r = c[0];
        __attribute__ ((aligned(32))) uint8_t c[1 + 32] = {};
        return r;
    }

    void bitFlipNaiveArrayll(uint64_t * bits, uint32_t size) {
        for (uint32_t i = 0; i < size; i++) bits[i] = bitFlipNaive<uint64_t>(bits[i]);
    }

    void bitFlipTableArrayl(uint32_t * bits, uint32_t size) {
        for (uint32_t i = 0; i < size; i++) bits[i] = bitFlipTable<uint32_t>(bits[i]);
    }

    void bitFlipTableArrays(uint16_t * bits, uint32_t size) {
        for (uint32_t i = 0; i < size; i++) bits[i] = bitFlipTable<uint16_t>(bits[i]);
    }

    void bitFlipMaskArrayb(uint8_t * bits, uint32_t size) {
        for (uint32_t i = 0; i < size; i++) bits[i] = bitFlipMask<uint8_t> (bits[i]);
    }

    void bitFlipNaiveArrayb(uint8_t * bits, uint32_t size) {
        for (uint32_t i = 0; i < size; i++) bits[i] = bitFlipNaive<uint8_t> (bits[i]);
    }

    __m256 multiply_and_add(__m256 a, __m256 b, __m256 c) {

        return _mm256_fmadd_ps(a, b, c);
    }

}


#endif