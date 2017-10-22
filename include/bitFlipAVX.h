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
 * File:   BitReverseAVX.h
 * Author: Anthony Campbell
 *
 * Created on 21 October 2017, 05:34
 */

#ifndef BITREVERSEAVX_H
    #define BITREVERSEAVX_H

    #include "bitFlip.h"
    #include <stdint.h>
    #include <cmath>
    #include <type_traits>
    #include <typeinfo>
    #include <memory>
    #include <string>
    #include <cstdlib>
    #include <iostream>
    #include <iomanip>
    #include <bitset>
    #include <limits>
    #include<immintrin.h>



    #ifdef __cplusplus
extern "C" {
    #endif


    void print256_8(__m256i var);
    void print256_16(__m256i var);
    void print256_32(__m256i var);
    void print256_64(__m256i var);

    uint8_t flipAVX(uint8_t bits);

    #ifdef __cplusplus
}
namespace bits {

    __attribute__ ((aligned(32))) static uint8_t k1[32 * 3] = {
        0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F,
        0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E, 0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F, 0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E, 0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F,
        0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0, 0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0, 0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0, 0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0
    };


    __attribute__ ((aligned(32))) static uint8_t a1[32] = {
        0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F
    };
    __attribute__ ((aligned(32))) static uint8_t a2[32] = {
        0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E, 0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F, 0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E, 0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F
    };
    __attribute__ ((aligned(32))) static uint8_t a3[32] = {
        0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0, 0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0, 0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0, 0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0
    };

    __attribute__ ((aligned(32))) static uint8_t o256[32] = {
        31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
    };

    __attribute__ ((aligned(32))) static uint8_t o16[32] = {
        1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14, 17, 16, 19, 18, 21, 20, 23, 22, 25, 24, 27, 26, 29, 28, 31, 30
    };

    __attribute__ ((aligned(32))) static uint8_t o32[32] = {
        3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12, 19, 18, 17, 16, 23, 22, 21, 20, 27, 26, 25, 24, 31, 30, 29, 28
    };

    __attribute__ ((aligned(32))) static uint8_t o64[32] = {
        7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8, 23, 22, 21, 20, 19, 18, 17, 16, 31, 30, 29, 28, 27, 26, 25, 24
    };


    static const __m256i ia1 = _mm256_load_si256((__m256i const*) a1);
    static const __m256i ia2 = _mm256_load_si256((__m256i const*) a2);
    static const __m256i ia3 = _mm256_load_si256((__m256i const*) a3);
    static const __m256i io16 = _mm256_load_si256((__m256i const*) o16);
    static const __m256i io32 = _mm256_load_si256((__m256i const*) o32);
    static const __m256i io64 = _mm256_load_si256((__m256i const*) o64);

    __m256i flipAVX256(__m256i * ag);

    /* NB remember to pack data into 256 byte chunks*/
    template<class T>
    __m256i get256(T * input) {
        return _mm256_load_si256((__m256i const*) input);
    }

    /* 256 Bytes =   4 * 64
                                     8 * 32
                                   16 * 16
                                   32 * 32
     */
    template<class T>
    __m256i flipAVX(T * input) {

        __m256i acc1;
        __m256i arg1;

        arg1 = _mm256_load_si256((__m256i const*) input);

        acc1 = _mm256_and_si256(ia1, arg1); // acc1 = arg1 and ia1
        arg1 = _mm256_andnot_si256(ia1, arg1); //arg1 = arg1 and not ia1
        arg1 = _mm256_srli_epi32(arg1, 4); //shift arg1 4h
        arg1 = _mm256_shuffle_epi8(ia2, arg1); //arg1 = shuf arg1 and ia2
        acc1 = _mm256_shuffle_epi8(ia3, acc1); // acc1 =  shuf acc1 and ia3
        if ((std::numeric_limits<T>::digits == 8))
            return _mm256_or_si256(acc1, arg1);
        else {
            arg1 = _mm256_or_si256(acc1, arg1);
            if ((std::numeric_limits<T>::digits == 16))
                return _mm256_shuffle_epi8(_mm256_or_si256(acc1, arg1), io16);
            if ((std::numeric_limits<T>::digits == 32))
                return _mm256_shuffle_epi8(_mm256_or_si256(acc1, arg1), io32);
            if ((std::numeric_limits<T>::digits == 64))
                return _mm256_shuffle_epi8(_mm256_or_si256(acc1, arg1), io64);

        }
    }

    template <typename T, size_t N> void flipAVXArray(T(&a)[N]) {
        if ((std::numeric_limits<T>::digits == 16)) {
            _bitflipbyte((uint8_t*) a, (uint8_t) ceil((N * 2) / 32.0), bits::k1);
            uint16_t b[N];
            for (int i = 0; i < N; i++)
                b[i] = __builtin_bswap16(a[i]);
            std::copy(std::begin(b), std::end(b), std::begin(a));
        }
        if ((std::numeric_limits<T>::digits == 8))
            _bitflipbyte((uint8_t*) a, (uint8_t) ceil(N / 32.0), bits::k1);
    }

    template<class T>
    void print256(__m256i var) {
        T *val = (T*) & var;
        constexpr uint8_t size = sizeof ( T);
        constexpr uint8_t size8 = sizeof ( T) * 8;
        for (uint8_t i = 0; i < (32 / size); i++)
            std::cout << std::bitset < size8 > (val[i]) << std::endl;

    }

    template<class T>
    void i256toa(__m256i var, T * a) {
        T *val = (T*) & var;
        constexpr uint8_t size = sizeof ( T);
        constexpr uint8_t size8 = sizeof ( T) * 8;
        for (uint8_t i = 0; i < (32 / size); i++) {
            a[i] = static_cast<T> (var[i] << size8);
        }

    }

    template<class S>
    class AVX {
    public:
        explicit AVX(__m256i v) : var(v) {
            m256i = true;
        };
        explicit AVX(S * a) : arr(a) {
            m256i = false;
        };
    private:
        __m256i var;
        S * arr;
        bool m256i;
        template<class T>
        friend std::ostream & operator<<(std::ostream& s, bits::AVX<T> & p);
    };

    template<class T>
    std::ostream & operator<<(std::ostream& s, bits::AVX<T>& p) {
        constexpr uint8_t TS = sizeof ( T);

        if (!p.m256i)
            p.var = bits::flipAVX(p.arr);

        T * val = (T *) & p.var;
        for (int i = 0; i < 4; i++)
            s << std::bitset<64>(val[i]) << std::endl;

        return s;
    };

}

    #endif
#endif /* BITREVERSEAVX_H */

