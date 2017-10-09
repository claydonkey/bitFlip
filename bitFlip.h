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




    #ifdef __cplusplus
	#include <type_traits>
	#include <stdint.h>
	#include <iostream>
	#include <stdint.h>

template <typename T>
typename std::enable_if<std::is_integral<T>::value, T >::type bitFlip(T const &i) {
    T r;
    uint8_t c, s = sizeof (r)*8;
    for (; c < s; c++) r |= ((i >> c) & 1) << (s - c - 1);
    return r;
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value, uint8_t>::type bits(T const &o) {
    T n = o;
    uint8_t c(0);
    while (n) {
	n = n & (n - 1);
	c++;
    }
    return c;
}
    #endif

    #define DISPLAY_HEIGHT  4
    #define DISPLAY_WIDTH   32
    #define NUM_DATA_BYTES  400000000


    #ifdef __cplusplus
extern "C" {
    #endif

    __attribute__ ((aligned(32))) static uint8_t k1[32 * 3] = {
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x00, 0x08, 0x04, 0x0c, 0x02, 0x0a, 0x06, 0x0e, 0x01, 0x09, 0x05, 0x0d, 0x03, 0x0b, 0x07, 0x0f, 0x00, 0x08, 0x04, 0x0c, 0x02, 0x0a, 0x06, 0x0e, 0x01, 0x09, 0x05, 0x0d, 0x03, 0x0b, 0x07, 0x0f,
	0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0, 0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0
    };

    // The data to be bitflipped (+32 to avoid the quantization out of memory problem)
    __attribute__ ((aligned(32))) static uint8_t data[NUM_DATA_BYTES + 32] = {};

    extern void bitflipbyte(uint8_t[], uint32_t, uint8_t[]);
    //extern void bitflipbyte(unsigned char *, unsigned int, unsigned char *);

    #ifdef __cplusplus
}
    #endif


    #ifdef __cplusplus
extern "C" {
    #endif

    uint64_t bitFlipll(uint64_t o);
    uint16_t bitFlipl(uint16_t o);
    uint8_t bitFlipb(uint8_t o);

    #ifdef __cplusplus
}
    #endif

#endif /* bitFlip_H */
