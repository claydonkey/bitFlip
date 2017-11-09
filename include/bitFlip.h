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

#ifndef include_bitflip_H
  #define include_bitflip_H

  #include <stdint.h>
  #include "bitFlipTables.h"

  #ifdef __cplusplus

	#include <type_traits>
	#include <iostream>
	#include <limits>
	#include <cmath>
	#include <immintrin.h>
	#include <functional>

extern "C" {

  extern void _bitflipbyte(uint8_t[], uint32_t, uint8_t[]);
  extern void _bitflipllloop(uint64_t * n, uint32_t size);

  typedef uint16_t v4si __attribute__ ((vector_size(16)));
  typedef __attribute__ ((aligned(32))) uint8_t aligned_uint8_t;

  uint16_t flipNaives(uint16_t bits);
  uint64_t flipNaivell(const uint64_t n);
  uint64_t flipMask(uint64_t bits);
  uint8_t popcntHAKMEM16(uint16_t i);
  uint8_t popcntHAKMEM32(uint32_t i);
  uint8_t popcntHAKMEM64(uint64_t i);
  uint8_t popcntSWAR64(uint64_t i);

  void flipNaiveArrayll(uint64_t * n, uint32_t size);
  void flipTableArrayl(uint32_t * bits, uint32_t size);
  void flipTableArrays(uint16_t * bits, uint32_t size);
  void flipMaskArrayb(uint8_t * bits, uint32_t size);
  void flipNaiveArrayb(uint8_t * bits, uint32_t size);

  v4si builtinSwap(v4si x);
  v4si builtinSwap2();
}

namespace bits {

  template <class T, typename = typename std::enable_if<std::is_integral<T>::value, T >::type>
  auto flipNaiveLambda(T const &i) -> T {
	uint64_t r = 0;
	uint8_t c = 0;
	const uint8_t s = sizeof (i) * CHAR_BIT;
	return [&] (uint64_t i) -> uint64_t {
	  r = 0;
	  for (c = 0; c < s; c++) r |= ((i >> c) & 1) << (s - c - 1);
	  return r;
	}(i);

  }

  template <class T, typename = typename std::enable_if<std::is_integral<T>::value, T >::type>
  auto flipMask(T const &bits) ->T {
	if ((std::numeric_limits<T>::digits == 8))
	  return ( (bits * 0x202020202ULL & 0x010884422010ULL) % 1023);
  }

  template <class T, typename = typename std::enable_if<std::is_integral<T>::value, T >::type>
  auto flipTable(T const &i) -> T {
	if ((std::numeric_limits<T>::digits == 32))
	  return (bitFlipTable8[i & 0xff] << 24) | (bitFlipTable8[(i >> 8) & 0xff] << 16) | (bitFlipTable8[(i >> 16) & 0xff] << 8) | (bitFlipTable8[(i >> 24) & 0xff]);
	if ((std::numeric_limits<T>::digits == 16))
	  return (bitFlipTable16[i]);
	if ((std::numeric_limits<T>::digits == 8))
	  return (bitFlipTable8[i]);
  }

  template <class T, typename = typename std::enable_if<std::is_integral<T>::value, T >::type>
  auto flipNaive(T const &i) ->T {
	T r;
	uint8_t c, s;
	for (r = 0, c = 0, s = sizeof (T) * CHAR_BIT; c < s; c++) r |= ((i >> c) & 1) << (s - c - 1);
	return r;
  }
  //http://coliru.stacked-crooked.com/a/852f5ec2d629e0b8
  template<class... Ts>
  using flipF = auto(Ts...) -> decltype(bits::flipNaive(std::declval<Ts>()...));


  template<class... Ts>
  using B = decltype(FlipNaive(std::declval<Ts>()...))(Ts...);

  template <class T, size_t N>
  void flipNaiveLambdaArray(T(&a)[N]) {
	for (uint32_t i = 0; i < N; i++) a[i] = flipNaiveLambda(a[i]);
  }

  template <class T, size_t N>
  void flipTableArray(T(&bits)[N]) {
	for (uint32_t i = 0; i < N; i++) bits[i] = flipTable<T>(bits[i]);
  }

  template <class T, size_t N>
  void flipMaskArray(T(&bits)[N]) {
	//for 16bit ints only
	for (uint32_t i = 0; i < N; i++) bits[i] = ((bits[i] * 0x202020202ULL & 0x010884422010ULL) % 1023);
  }

  template <class T, size_t N>
  void flipNaiveArray(T(&bits)[N]) {
	uint64_t r, i, j = 0;
	for (; j < N; j++) {
	  for (r = 0, i = 0; i < sizeof (T) * CHAR_BIT; ++i) r |= ((bits[j] >> i) & 1) << ((sizeof (T) * CHAR_BIT) - i - 1);
	  bits[j] = r;
	}
  }

  template <class B, class T, size_t N >
  void flipArray(T(&a)[N], B f) {
	for (uint32_t i = 0; i < N; i++) a[i] = f(a[i]);
  }
  /// Return population count of on bits in integer

  /**
   * @brief Returns population count of <b>on</b> bits in std::integer using a naive implementation
   * @params i: std:integer
   * @return   uint8_t
   * @note The Wegner function could be competitive when the population count is relatively low
   * (e.g., less than 4 one bit per 64 - bit word). When the population count is expected to be high
   * (e.g., more than 60 one bit per 64 - bit words)
   * one could simply negate the words prior to using the function so as to count the number of zeros instead.
   * The core insight behind the Wegner function is that the line of C code
   * n &= n - 1 sets to zero the least signicant bit of x, as one can readily check.
   **/
  template <class T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
  uint8_t naivepopcnt(T const &i) {
	uint8_t cnt;
	for (T c(0); c<sizeof (T) * CHAR_BIT; c++) {
	  if (1 & c) cnt++;
	  c >> 1;
	}
	return cnt;
  }

  /**
   * @brief Returns population count of <b>on</b> bits in std::integer using a wegner's algorithm
   * @params i: std:integer
   * @return  uint8_t
   * @note The Wegner function could be competitive when the population count is relatively low
   * (e.g., less than 4 one bit per 64 - bit word). When the population count is expected to be high
   * (e.g., more than 60 one bit per 64 - bit words)
   * one could simply negate the words prior to using the function so as to count the number of zeros instead.
   * The core insight behind the Wegner function is that the line of C code
   * n &= n - 1 sets to zero the least signicant bit of x, as one can readily check.
   **/
  template <class T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
  uint8_t popcntWegner(T const &i) {
	uint8_t c(0);
	for (T n(i); n; c++)
	  n &= (n - 1);
	return c;
  }

  /// Recursive Lambda to return population count of \bon bits in integer
  /**
   * @brief Returns population count of <b>on</b> bits in std::integer using wegner's alg inside a recursive lambda
   * @params n: std:integer, c counter (set to 0)
   * @return   T type of std::integer
   * @note The Wegner function could be competitive when the population count is relatively low
   * (e.g., less than 4 one bit per 64 - bit word). When the population count is expected to be high
   * (e.g., more than 60 one bit per 64 - bit words)
   * one could simply negate the words prior to using the function so as to count the number of zeros instead.
   * The core insight behind the Wegner function is that the line of C code
   * n &= n - 1 sets to zero the least signicant bit of n, as one can readily check.
   **/
  template<class T>
  std::function<T(T, uint8_t) > rpop_cnt_wegner = [&](T n, uint8_t c) ->uint8_t {
	n &= (n - 1);
	return n ? rpop_cnt_wegner<T>(n, ++c) : ++c;
  };

  template<class T>
  std::function<T(T) > pop_cnt_wegner = [&](T n) ->uint8_t {
	n &= (n - 1);
	return n ? rpop_cnt_wegner<T>(n, 1) : 0;
  };

  /// Return population count of \bon bits in integer using AVX instructions

  /**
   * @brief Returns population count of <b>on</b> bits in std::integer using AVX instructions
   * @params i: std:integer
   * @return   T type of std::integer
   * @note Using the intel AVX instructions
   **/
  template <class T, typename = typename std::enable_if<std::is_integral<T>::value, uint8_t>::type>
  T popcntIntrinsic(T const &i) {
	if ((std::numeric_limits<T>::digits == 64))
	  return _mm_popcnt_u64(i);
	if ((std::numeric_limits<T>::digits == 32))
	  return _mm_popcnt_u32(i);
  }
} //end namespace bits
  #endif
#endif  // include_flip_H
