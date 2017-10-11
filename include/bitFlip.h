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

#define DISPLAY_HEIGHT  4
#define DISPLAY_WIDTH   32
//  #define NUM_DATA_BYTES  400000000
#define NUM_DATA_BYTES  4




#ifdef __cplusplus

extern "C" {
    __attribute__ ((aligned(32))) static uint8_t data[NUM_DATA_BYTES + 32] = {};

    extern void bitflipbyte(uint8_t[], uint32_t, uint8_t[]);
    extern uint16_t bitfliplong(uint64_t);
    extern void bitflipllloop(uint64_t * n, uint32_t size);

    // The data to be bitflipped (+32 to avoid the quantization out of memory problem)

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

__attribute__ ((aligned(32))) static uint8_t k1[32 * 3] = {
    0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F,
    0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E, 0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F, 0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E, 0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F,
    0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0, 0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0, 0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0, 0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0
};

static const uint8_t BitReverseTable256[] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};


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
        return (BitReverseTable256[i & 0xff] << 24) | (BitReverseTable256[(i >> 8) & 0xff] << 16) | (BitReverseTable256[(i >> 16) & 0xff] << 8) | (BitReverseTable256[(i >> 24) & 0xff]);
    if ((std::numeric_limits<T>::digits == 16))
        return (BitReverseTable16[i]);
    if ((std::numeric_limits<T>::digits == 8))
        return (BitReverseTable256[i]);
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
