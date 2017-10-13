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
 * File:   bitFlip.h
 * Author: Anthony Campbell
 *
 * Created on 05 October 2017, 11:13
 */

#ifndef bitFlip_H
    #define bitFlip_H

    #include <stdint.h>


    #ifdef __cplusplus

extern "C" {


    extern void bitflipbyte(uint8_t[], uint32_t, uint8_t[]);
    extern uint16_t bitfliplong(uint64_t);
    extern void bitflipllloop(uint64_t * n, uint32_t size);
    uint16_t bitFlipNaives(uint16_t bits);
    uint64_t bitFlipNaivell(const uint64_t n);
    uint8_t bitFlipZb(uint8_t bits);
    void bitFlipNaiveArrayll(uint64_t * n, uint32_t size);
    void bitFlipTableArrayl(uint32_t * bits, uint32_t size);
    void bitFlipTableArrays(uint16_t * bits, uint32_t size);
    void bitFlipMaskArrayb(uint8_t * bits, uint32_t size);
    void bitFlipNaiveArrayb(uint8_t * bits, uint32_t size);

}

        #include <type_traits>
        #include <stdint.h>
        #include <iostream>
        #include <limits>
        #include <cmath>
        #include "BitReverseTable16.h"
        #include "BitReverseTable8.h"

__attribute__ ((aligned(32))) static uint8_t k1[32 * 3] = {
    0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F,
    0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E, 0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F, 0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E, 0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F,
    0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0, 0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0, 0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0, 0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0
};


void bitFlipNaiveLambda(uint64_t * bits, uint32_t size);
typedef __attribute__ ((aligned(32))) uint8_t aligned_uint8_t;

template <typename T, size_t N> void bitFlipZ(T(&a)[N]) {
    if ((std::numeric_limits<T>::digits == 16))
        bitflipbyte((uint8_t*) a, (uint8_t) ceil(N / 32.0), k1);
    else
        if ((std::numeric_limits<T>::digits == 8))
        bitflipbyte((uint8_t*) a, (uint8_t) ceil(N / 32.0), k1);
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value, T >::type bitFlipTable(T const &i) {
    if ((std::numeric_limits<T>::digits == 32))
        return (BitReverseTable8[i & 0xff] << 24) | (BitReverseTable8[(i >> 8) & 0xff] << 16) | (BitReverseTable8[(i >> 16) & 0xff] << 8) | (BitReverseTable8[(i >> 24) & 0xff]);
    if ((std::numeric_limits<T>::digits == 16))
        return (BitReverseTable16[i]);
    if ((std::numeric_limits<T>::digits == 8))
        return (BitReverseTable8[i]);
}

template <typename T, size_t N> void bitFlipTableArray(T(&bits)[N]) {
    for (uint32_t i = 0; i < N; i++) bits[i] = bitFlipTable<T>(bits[i]);
}

template <typename T, size_t N> void bitFlipMaskArray(T(&bits)[N]) {
    for (uint32_t i = 0; i < N; i++) bits[i] = ((bits[i] * 0x202020202ULL & 0x010884422010ULL) % 1023);
}

template <typename T, size_t N> void bitFlipNaiveArray(T(&bits)[N]) {
    uint64_t r, i, j = 0;
    for (; j < N; j++) {
        for (r = 0, i = 0; i < sizeof (T)*8; ++i) r |= ((bits[j] >> i) & 1) << ((sizeof (T)*8) - i - 1);
        bits[j] = r;
    }
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value, T >::type bitFlipNaive(T const &i) {
    T r = 0;
    uint8_t c = 0;
    uint8_t s = sizeof (T)*8;
    for (; c < s; c++) r |= ((i >> c) & 1) << (s - c - 1);
    return r;
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value, uint8_t>::type bitCount(T const &i) {
    T n = i;
    uint8_t c(0);
    while (n) {
        n = n & (n - 1);
        c++;
    }
    return c;
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value, T >::type bitFlipMask(T const &bits) {
    if ((std::numeric_limits<T>::digits == 8))
        return ( (bits * 0x202020202ULL & 0x010884422010ULL) % 1023);
}

    #endif
#endif /* bitFlip_H */
