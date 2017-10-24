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

/*
namespace bits {

    enum displayFlags {
        NONE = 0,
        DEBUG = 1,
        INPUT = 2
    };

    template<class T>
    void printBitFlipRange(uint64_t range[], char mF = 'B', uint8_t mH = 0) {

        __m256i flip;
        const uint16_t _IntSize = sizeof (T);
        const uint16_t _Step = 32 / _IntSize;
        T _Buffer = ceil((range[1] - range[0]) / 4) + 1;
        const T _ArrayLength = _Buffer * 4;

        __attribute__ ((aligned(32))) T input[_ArrayLength ];

        if (DEBUG & mH) {
            std::cout << "--------------------------------------------------------------------------------------------" << std::endl;
            std::cout
                    << " --- Pushing in m256i "
                    << ".\n --- Range size :" << (range[1] - range[0])
                    << ".\n --- Buffer: " << +_Buffer
                    << ".\n --- Step: " << +_Step
                    << ".\n --- ArrayLength: " << +_ArrayLength
                    << ".\n --- IntSize (Integer Length): " << +_IntSize << std::endl;
        }

        std::cout << "--------------------------------------------------------------------------------------------" << std::endl;

        for (T i = range[0], cnt = 0; i <= range[1]; i++, cnt++) {
            input[cnt] = i;
            if (INPUT & mH) {
                std::cout << std::setfill('0') << std::setw(3 * _IntSize * CHAR_BIT / 8) << +input[cnt] << " ";
                if (!((i + 1) % (24 / _IntSize)) && i != 0) std::cout << std::endl;
            }
        }
        std::cout << std::endl << std::endl;

        for (T i = 0; i < _ArrayLength; i += _Step) {

            //        flip = bits::flipAVXzCnt(&input[i]);
        //    if (mF=='V' || mF=='v' || mF ='Z' )
            bits::AVX<T> p(&input[i]);
            p.mFormat = mF;
            std::cout << p;

            //    bits::print256<T>(flip);
        }
    }
}
 * */
#endif /* CONSOLE_H */

