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

#include <iostream>
#include "bitFlip.h"
#include "bitFlipAVX.h"
#include <stdlib.h>
#include <stdint.h>
#include <cerrno>
#include <cstring>
#include <bitset>
#include <math.h>
#include <omp.h>
#include  <iomanip>

using namespace std;

int main(int argc, const char** argv) {


    bool upper = false;
    char * pEnd;
    const char *nptr;
    uint32_t range[2] = {0, 0};
    /*
        argc = 3;
        const char * argvN[3] = {"1", "10", "128"};
        const char ** pargvN = argvN;
        argv = pargvN;
     */
    if (argc > 2) {

        errno = 0;
        pEnd = NULL;
        nptr = argv[2];

        range[1] = strtol(nptr, &pEnd, 10);

        if (nptr == pEnd) {
            printf(" number: %lu  invalid  (no digits found, 0 returned)\n", range[1]);
            return 0;
        }
        upper = true;

    }

    if (argc > 1) {

        errno = 0;
        pEnd = NULL;
        nptr = argv[1];

        range[0] = strtol(nptr, &pEnd, 10);

        if (nptr == pEnd) {
            printf(" number: %lu  invalid  (no digits found, 0 returned)\n", range[0]);
            return 0;
        }

        if (upper && (range[1] < range[0])) {
            printf("number range: %lu is less than %lu (range reversed) \n", range[1], range[0]);
            swap(range[1], range[0]);
        }

    } else {
        fprintf(stderr, "\n Error: insufficient input. Usage: %s int [int]\n\n", argv[0]);
        return 0;
    }

#define INTTYPE uint64_t

    const uint16_t intsize = sizeof (INTTYPE);
    const uint16_t arraystep = 256 / (8 * intsize);
    INTTYPE BUFFERCNT = ceil((range[1] - range[0]) / arraystep) + 1;

    std::cout << "Range size :" << (range[1] - range[0])
            << "\nBUFFERCNT: " << BUFFERCNT << ".\nArray step size: "
            << arraystep << ".\nByte-count of Integer: " << intsize
            << ".\nArray Size: " << (BUFFERCNT * arraystep) << std::endl;

    __attribute__ ((aligned(32))) INTTYPE input[arraystep * BUFFERCNT ];
    INTTYPE j;
    for (INTTYPE i = range[0], j = 0; i <= range[1]; i++, j++) {
        input[j] = i;
        std::cout << std::setfill('0') << std::setw(6) << input[j] << " ";
        if (!((i + 1) % 16) && i != 0) std::cout << endl;
    }

    std::cout << endl;

    for (INTTYPE i = 0; i < (arraystep * BUFFERCNT); i = i + arraystep) {

        bits::AVX<INTTYPE> p(&input[i]);
        p.mBinaryDisplay = false;
        cout << std::setfill('0') << std::setw(2) << i << "\n" << p;

    }
    return 0;

}
