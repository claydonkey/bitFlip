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

#include "bitFlipAVX.h"
#include <stdint.h>
#include <immintrin.h>
#include <iostream>
#include <bitset>

#ifdef __cplusplus
extern "C" {
#endif

  __attribute__ ((aligned(32))) uint8_t c[1 + 32] = {};

  void print256_8(__m256i var) {
	uint8_t *val = reinterpret_cast<uint8_t*> (& var);

	std::cout <<
			"------------------------------------------------------------" <<
			std::endl;
	for (int i = 0; i < 32; i++) {
	  std::cout << std::bitset<8>(val[i]) << std::endl;
	}
	std::cout << std::endl;
  }

  void print256_16(__m256i var) {
	uint16_t *val = reinterpret_cast<uint16_t*> (& var);

	std::cout <<
			"------------------------------------------------------------"
			<< std::endl;
	for (int i = 0; i < 16; i++) {
	  std::cout << std::bitset<16>(val[i]) << std::endl;
	}
	std::cout << std::endl;
  }

  void print256_32(__m256i var) {
	uint32_t *val = reinterpret_cast<uint32_t*> (& var);

	std::cout <<
			"------------------------------------------------------------"
			<< std::endl;
	for (int i = 0; i < 8; i++) {
	  std::cout << std::bitset<32>(val[i]) << std::endl;
	}
	std::cout << std::endl;
  }

  void print256_64(__m256i var) {
	uint64_t *val = reinterpret_cast<uint64_t*> (& var);

	std::cout
			<< "------------------------------------------------------------"
			<< std::endl;
	for (int i = 0; i < 4; i++)
	  std::cout << std::bitset<64>(val[i]) << std::endl;
	std::cout << std::endl;
  }

  uint8_t flipAVX8(uint8_t bits) {
	uint8_t r = 0;
	c[0] = bits;
	bits::flipAVXArray<uint8_t> (c);
	r = c[0];
	return r;
  }

#ifdef __cplusplus
}
#endif
namespace bits {

  __m256i flipAVX256(__m256i * ag) {
	__m256i acc1;
	__m256i arg1;

	__m256i arg = *ag;

	acc1 = _mm256_and_si256(ia1, arg1); // acc1 = arg1 and ia1
	arg1 = _mm256_andnot_si256(ia1, arg1); // arg1 = arg1 and not ia1
	arg1 = _mm256_srli_epi32(arg1, 4); // shift arg1 4h
	arg1 = _mm256_shuffle_epi8(ia2, arg1); // arg1 = shuf arg1 and ia2
	acc1 = _mm256_shuffle_epi8(ia3, acc1); // acc1 =  shuf acc1 and ia3

	//  return _mm256_or_si256(acc1, arg1);
	return _mm256_shuffle_epi8(_mm256_or_si256(acc1, arg1), io64);
  }
} // namespace bits
