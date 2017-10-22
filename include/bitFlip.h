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
 * File:   flip.h
 * Author: Anthony Campbell
 *
 * Created on 05 October 2017, 11:13
 */

#ifndef flip_H
    #define flip_H

    #include <stdint.h>
    #include "bitFlipTables.h"

    #ifdef __cplusplus

        #include <type_traits>
        #include <iostream>
        #include <limits>
        #include <cmath>
        #include <immintrin.h>
extern "C" {

    extern void _bitflipbyte(uint8_t[], uint32_t, uint8_t[]);
    extern void _bitflipllloop(uint64_t * n, uint32_t size);

    uint16_t flipNaives(uint16_t bits);
    uint64_t flipNaivell(const uint64_t n);

    uint64_t flipMask(uint64_t bits);

    int16_t popcnt16(uint16_t i);
    int32_t popcnt32(uint32_t i);

    void flipNaiveArrayll(uint64_t * n, uint32_t size);
    void flipTableArrayl(uint32_t * bits, uint32_t size);
    void flipTableArrays(uint16_t * bits, uint32_t size);
    void flipMaskArrayb(uint8_t * bits, uint32_t size);
    void flipNaiveArrayb(uint8_t * bits, uint32_t size);
    typedef uint16_t v4si __attribute__ ((vector_size(16)));
    typedef __attribute__ ((aligned(32))) uint8_t aligned_uint8_t;
    v4si builtinSwap(v4si x);
    v4si builtinSwap2();
}



namespace bits {

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, T >::type flipNaiveLambda(T const &i) {
        uint64_t r = 0;
        uint8_t c = 0;
        const uint8_t s = sizeof (T)*8;
        return [&] (uint64_t i) -> uint64_t {
            r = 0;
            for (c = 0; c < s; c++) r |= ((i >> c) & 1) << (s - c - 1);
            return r;
        }(i);

    }

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, T >::type flipMask(T const &bits) {
        if ((std::numeric_limits<T>::digits == 8))
            return ( (bits * 0x202020202ULL & 0x010884422010ULL) % 1023);
    }

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, T >::type flipTable(T const &i) {
        if ((std::numeric_limits<T>::digits == 32))
            return (bitFlipTable8[i & 0xff] << 24) | (bitFlipTable8[(i >> 8) & 0xff] << 16) | (bitFlipTable8[(i >> 16) & 0xff] << 8) | (bitFlipTable8[(i >> 24) & 0xff]);
        if ((std::numeric_limits<T>::digits == 16))
            return (bitFlipTable16[i]);
        if ((std::numeric_limits<T>::digits == 8))
            return (bitFlipTable8[i]);
    }

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, T >::type
    flipNaive(T const &i) {
        T r;
        uint8_t c, s;
        for (r = 0, c = 0, s = sizeof (T)*8; c < s; c++) r |= ((i >> c) & 1) << (s - c - 1);
        return r;
    }
    //http://coliru.stacked-crooked.com/a/852f5ec2d629e0b8
    template<typename... Ts>
    using flipF = auto(Ts...) -> decltype(bits::flipNaive(std::declval<Ts>()...));


    template<typename... Ts>
    using B = decltype(FlipNaive(std::declval<Ts>()...))(Ts...);

    template <typename T, size_t N> void flipNaiveLambdaArray(T(&a)[N]) {
        for (uint32_t i = 0; i < N; i++) a[i] = flipNaiveLambda(a[i]);
    }

    template <typename T, size_t N> void flipTableArray(T(&bits)[N]) {
        for (uint32_t i = 0; i < N; i++) bits[i] = flipTable<T>(bits[i]);
    }

    template <typename T, size_t N> void flipMaskArray(T(&bits)[N]) {
        //for 16bit ints only
        for (uint32_t i = 0; i < N; i++) bits[i] = ((bits[i] * 0x202020202ULL & 0x010884422010ULL) % 1023);
    }

    template <typename T, size_t N> void flipNaiveArray(T(&bits)[N]) {
        uint64_t r, i, j = 0;
        for (; j < N; j++) {
            for (r = 0, i = 0; i < sizeof (T)*8; ++i) r |= ((bits[j] >> i) & 1) << ((sizeof (T)*8) - i - 1);
            bits[j] = r;
        }
    }

    template <class B, class T, size_t N > void flipArray(T(&a)[N], B f) {
        for (uint32_t i = 0; i < N; i++) a[i] = f(a[i]);
    }
    //The Wegner function

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, uint8_t>::type popcntWegner(T const &i) {
        T n = i;
        uint8_t c(0);
        while (n) {
            n = n & (n - 1);
            c++;
        }
        return c;
    }

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, T >::type popcntIntrinsic(T const &i) {
        if ((std::numeric_limits<T>::digits == 32))
            return _mm_popcnt_u32(i);
        if ((std::numeric_limits<T>::digits == 64))
            return _mm_popcnt_u64(i);

    }


}
    #endif
#endif /* flip_H */
