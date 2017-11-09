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
#include "bitFlipTables.h"
#include <immintrin.h>
#include <functional>

#ifdef __cplusplus

#ifdef NOASSMBLR

void _bitflipbyte(uint8_t[], uint32_t, uint8_t[]) {
}
#endif

#if defined (__MINGW32__) || defined (__unix__)

void _bitflipllloop(uint64_t * bits, uint32_t size) {
  for (uint32_t i = 0; i < size; i++) bits[i] = bits::flipNaive<uint64_t>(bits[i]);
}
#endif

uint8_t flipMask(uint8_t bits) {
  return (bits * 0x202020202ULL & 0x010884422010ULL) % 1023;
}

extern "C" {

  uint16_t flipNaives(const uint16_t bits) {
	return bits::flipNaive<uint16_t> (bits);
  }

  uint64_t flipNaivell(const uint64_t bits) {
	return bits::flipNaive<uint64_t> (bits);
  }

  void flipNaiveArrayll(uint64_t * bits, uint32_t size) {
	for (uint32_t i = 0; i < size; i++) bits[i] = bits::flipNaive<uint64_t>(bits[i]);
  }

  void flipTableArrayl(uint32_t * bits, uint32_t size) {
	for (uint32_t i = 0; i < size; i++) bits[i] = bits::flipTable<uint32_t>(bits[i]);
  }

  void flipTableArrays(uint16_t * bits, uint32_t size) {
	for (uint32_t i = 0; i < size; i++) bits[i] = bits::flipTable<uint16_t>(bits[i]);
  }

  void flipMaskArrayb(uint8_t * bits, uint32_t size) {
	for (uint32_t i = 0; i < size; i++) bits[i] = bits::flipMask<uint8_t> (bits[i]);
  }

  void flipNaiveArrayb(uint8_t * bits, uint32_t size) {
	for (uint32_t i = 0; i < size; i++) bits[i] = bits::flipNaive<uint8_t> (bits[i]);
  }

  __m256 multiply_and_add(__m256 a, __m256 b, __m256 c) {
	return _mm256_fmadd_ps(a, b, c);
  }

  v4si builtinSwap(v4si x) {
	static const v4si m = {7u, 6u, 5u, 4u, 3u, 2u, 1u, 0u};
	return __builtin_shuffle(x, m);
  }

  v4si builtinSwap2() {
	static const v4si x = {0x1234, 0x5678, 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE, 0xFFFF};
	static const v4si m = {7u, 6u, 5u, 4u, 3u, 2u, 1u, 0u};
	return __builtin_shuffle(x, m);
  }

  /**
   * @brief Returns population count of <b>on</b> bits in std::integer
   * @params i: std:integer
   * @return  uint8_t
   * @note SWAR as acronym for SIMD Within A Register was coined by Hank Dietz and Randy Fisher
   * deals with counting bits of duos, to aggregate the duo-counts to nibbles and bytes inside one 64-bit register in parallel,
   * to finally sum all bytes together.
   * According to Donald Knuth [13], a parallel population count routine was already introduced in 1957
   * due to Donald B. Gillies and Jeffrey C. P. Miller in the first textbook on programming,
   * second edition: The Preparation of Programs for an Electronic Digital Computer,
   * by Maurice Wilkes, David Wheeler and Stanley Gill, pages 191–193
   **/
  uint8_t popcntSWAR64(uint64_t i) {
	i = i - ((i >> 1) & 0x5555555555555555); /* put count of each 2 bits into those 2 bits */
	i = (i & 0x3333333333333333) + ((i >> 2) & 0x3333333333333333); /* put count of each 4 bits into those 4 bits */
	i = (i + (i >> 4)) & 0x0f0f0f0f0f0f0f0f; /* put count of each 8 bits into those 8 bits */
	i = (i * 0x0101010101010101) >> 56; /* returns 8 most significant bits of x + (x<<8) + (x<<16) + (x<<24) + ...  */
	return (uint8_t) i;
  }

  /// Population count of on bits in integer

  /**
   * @brief Returns population count of <b>on</b> bits in std::integer
   * @params i: uint16_t
   * @return  uint8_t
   * @note T HAKMEM 169 -A similar technique was proposed by Bill Gosper et al. from Massachusetts Institute of Technology,
   * as published 1972 in Memo 239 (HAKMEM) [18] [19],
   * to add bit-trio- rather than duo populations consecutively,
   * and the 32 bit version relies on casting out 63.
   * Note that the constants in the code below have octal (base-8) digits,
   * originally written in Assembly for the PDP-6 [20].
   * An expanded 64-bit version, casting out 511 or 4095, is slightly less efficient than the binary SWAR version above.
   **/

  uint8_t popcntHAKMEM16(uint16_t i) {
	uint16_t c = i - ((i >> 1) & 033333333333) - ((i >> 2) & 011111111111);
	return ((c + (c >> 3)) & 030707070707) % 63;
  }
  /// Population count of on bits in integer

  /**
   * @brief Returns population count of <b>on</b> bits in std::integer
   * @params i: uint32_t
   * @return  uint8_t
   * @note T HAKMEM 169 -A similar technique was proposed by Bill Gosper et al. from Massachusetts Institute of Technology,
   * as published 1972 in Memo 239 (HAKMEM) [18] [19],
   * to add bit-trio- rather than duo populations consecutively,
   * and the 32 bit version relies on casting out 63.
   * Note that the constants in the code below have octal (base-8) digits,
   * originally written in Assembly for the PDP-6 [20].
   * An expanded 64-bit version, casting out 511 or 4095, is slightly less efficient than the binary SWAR version above.
   **/

  uint8_t popcntHAKMEM32(uint32_t i) {
	uint32_t c = i - ((i >> 1) & 033333333333) - ((i >> 2) & 011111111111);
	return ((c + (c >> 3)) & 030707070707) % 63;
  }
  /// Population count of on bits in integer

  /**
   * @brief Returns population count of <b>on</b> bits in std::integer
   * @params i: uint64_t
   * @return  uint8_t
   * @note T HAKMEM 169 -A similar technique was proposed by Bill Gosper et al. from Massachusetts Institute of Technology,
   * as published 1972 in Memo 239 (HAKMEM) [18] [19],
   * to add bit-trio- rather than duo populations consecutively,
   * and the 32 bit version relies on casting out 63.
   * Note that the constants in the code below have octal (base-8) digits,
   * originally written in Assembly for the PDP-6 [20].
   * An expanded 64-bit version, casting out 511 or 4095, is slightly less efficient than the binary SWAR version above.
   **/

  uint8_t popcntHAKMEM64(uint64_t i) {
	uint64_t c = i - ((i >> 1) & 0333333333333333333333) - ((i >> 2) & 01111111111111111111111);
	return ((c + (c >> 3)) & 0307070707070707070707) % 63;
  }


}
namespace bits {

}
#endif