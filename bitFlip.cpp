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

#ifdef __cplusplus

uint16_t bitFlipNaive(uint16_t &o) {
    return bitFlipNaive<uint16_t> (o);
}

uint8_t bitFlipNaiveb(uint8_t &o) {
    return bitFlipNaive<uint8_t> (o);
}

uint32_t bitFlipNaivel(uint32_t &o) {
    return bitFlipNaive<uint32_t> (o);
}

uint64_t bitFlipNaivell(uint64_t &o) {
    return bitFlipNaive<uint64_t> (o);
}

uint8_t bitFlipMaskb(uint8_t &o) {
    return ( (o * 0x202020202ULL & 0x010884422010ULL) % 1023);
}

void bitFlipMaskArray(uint8_t * o, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) o[i] = bitFlipMaskb(o[i]);
}

void bitFlipNaiveArray(uint8_t * o, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) o[i] = bitFlipNaive(o[i]);
}

extern "C" {

    uint16_t bitFlipNaive(uint16_t *o) {
        return bitFlipNaive<uint16_t> (*o);
    }

}
#endif