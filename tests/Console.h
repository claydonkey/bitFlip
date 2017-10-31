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
 * File:   Console.h
 * Author: Anthony Campbell
 *
 * Created on 23 October 2017, 10:20
 */

#ifndef CONSOLE_H
    #define CONSOLE_H

    #include <iostream>
    #include <stdlib.h>
    #include <stdint.h>
    #include <cerrno>
    #include <cstring>
    #include <bitset>
    #include <math.h>
    #include <omp.h>
    #include  <iomanip>
    #include  <limits>
    #include  <climits>
    #include "bitFlip.h"
    #include "bitFlipAVX.h"

template<class T>
void printFlipByte(int mDisplay, uint64_t range[]) {

    const uint16_t _TSize = sizeof (T);
    const uint16_t _Step = 32 * _TSize;
    T _Buffer = ceil((range[1] - range[0]) / _Step) + 1;

    std::cout << "Range size :" << (range[1] - range[0])
            << "\nBuffer: " << +_Buffer << ".\n_Buffer (size): "
            << _Step << ".\n_Step (Byte-count of Integer): " << _TSize
            << ".\n_Buffer*_Step (Array Size (bits)): " << (_Buffer * _Step) << std::endl;

    __attribute__ ((aligned(32))) T input[_Step * _Buffer ];
    T j;

    for (T i = range[0], j = 0; i <= range[1]; i++, j++) {
        input[j] = i;
        std::cout << std::setfill('0') << std::setw(3 * _TSize * CHAR_BIT / 8) << +input[j] << " ";
        if (!((i + 1) % 16) && i != 0) std::cout << std::endl;
    }

    std::cout << std::endl;

    for (T i = 0; i < (_Step * _Buffer); i += _Step) {
        bits::AVX<T> p(bits::flipAVX(&input[i]));
        p.mDisplay = mDisplay;
        std::cout << p << std::endl;
    }
}

#endif /* CONSOLE_H */

