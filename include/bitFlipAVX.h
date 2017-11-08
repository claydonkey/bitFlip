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

#ifndef INCLUDE_BITFLIPAVX_H_
  #define INCLUDE_BITFLIPAVX_H_

  #include <stdint.h>
  #include <immintrin.h>
  #include <cmath>
  #include <type_traits>
  #include <typeinfo>
  #include <memory>
  #include <string>
  #include <cstring>
  #include <cstdlib>
  #include <iostream>
  #include <iomanip>
  #include <bitset>
  #include <limits>
  #include <tuple>
  #include <vector>
  #include <algorithm>
  #include <utility>
  #include <condition_variable>
  #include "bitFlip.h"
  #ifdef __cplusplus
extern "C" {
  #endif

  void print256_8(__m256i var);
  void print256_16(__m256i var);
  void print256_32(__m256i var);
  void print256_64(__m256i var);

  uint8_t flipAVX8(uint8_t bits);

  #ifdef __cplusplus
}
namespace bits {

  __attribute__ ((aligned(32))) static uint8_t k1[32 * 3] = {
	0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F,
	0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F,
	0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F,
	0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F,
	0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E,
	0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F,
	0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E,
	0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F,
	0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0,
	0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0,
	0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0,
	0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0
  };


  __attribute__ ((aligned(32))) static const uint8_t a1[32] = {
	0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F,
	0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F,
	0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F,
	0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F
  };
  __attribute__ ((aligned(32))) static const uint8_t a2[32] = {
	0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E,
	0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F,
	0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E,
	0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F
  };
  __attribute__ ((aligned(32))) static const uint8_t a3[32] = {
	0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0,
	0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0,
	0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0,
	0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0
  };

  __attribute__ ((aligned(32))) static const uint8_t o256[32] = {
	31, 30, 29, 28, 27, 26, 25, 24,
	23, 22, 21, 20, 19, 18, 17, 16,
	15, 14, 13, 12, 11, 10, 9, 8,
	7, 6, 5, 4, 3, 2, 1, 0
  };

  __attribute__ ((aligned(32))) static const uint8_t o16[32] = {
	1, 0, 3, 2, 5, 4, 7, 6,
	9, 8, 11, 10, 13, 12, 15, 14,
	17, 16, 19, 18, 21, 20, 23, 22,
	25, 24, 27, 26, 29, 28, 31, 30
  };

  __attribute__ ((aligned(32))) static const uint8_t o32[32] = {
	3, 2, 1, 0, 7, 6, 5, 4,
	11, 10, 9, 8, 15, 14, 13, 12,
	19, 18, 17, 16, 23, 22, 21, 20,
	27, 26, 25, 24, 31, 30, 29, 28
  };

  __attribute__ ((aligned(32))) static const uint8_t o64[32] = {
	7, 6, 5, 4, 3, 2, 1, 0,
	15, 14, 13, 12, 11, 10, 9, 8,
	23, 22, 21, 20, 19, 18, 17, 16,
	31, 30, 29, 28, 27, 26, 25, 24
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

  typedef struct ZCntFlip {
	__m256i _Flip;
	int64_t _zCnt[4];
  } __ZCntFlip;

  template<class T >
  __m256i flipAVX(T * input) {
	__m256i acc1;
	__m256i arg1;

	arg1 = _mm256_load_si256((__m256i const*) input);

	acc1 = _mm256_and_si256(ia1, arg1); /* acc1 = arg1 and ia1 */
	arg1 = _mm256_andnot_si256(ia1, arg1); /* arg1 = arg1 and not ia1 */
	arg1 = _mm256_srli_epi32(arg1, 4); /* shift arg1 4h */
	arg1 = _mm256_shuffle_epi8(ia2, arg1); /* arg1 = shuf arg1 and ia2 */
	acc1 = _mm256_shuffle_epi8(ia3, acc1); /* acc1 =  shuf acc1 and ia3 */
	if ((std::numeric_limits<T>::digits == 8)) {
	  return _mm256_or_si256(acc1, arg1);
	}
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

  template<class T >
  void flippAVX(T * input, T output[], uint64_t idx) {
	__m256i acc1;
	__m256i arg1;
	__m256i ret;
	arg1 = _mm256_load_si256((__m256i const*) input);

	acc1 = _mm256_and_si256(ia1, arg1); /* acc1 = arg1 and ia1 */
	arg1 = _mm256_andnot_si256(ia1, arg1); /* arg1 = arg1 and not ia1 */
	arg1 = _mm256_srli_epi32(arg1, 4); /* shift arg1 4h */
	arg1 = _mm256_shuffle_epi8(ia2, arg1); /* arg1 = shuf arg1 and ia2 */
	acc1 = _mm256_shuffle_epi8(ia3, acc1); /* acc1 =  shuf acc1 and ia3 */
	if ((std::numeric_limits<T>::digits == 8)) {
	  ret = _mm256_or_si256(acc1, arg1);
	}
	else {
	  arg1 = _mm256_or_si256(acc1, arg1);
	  if ((std::numeric_limits<T>::digits == 16))
		ret = _mm256_shuffle_epi8(_mm256_or_si256(acc1, arg1), io16);
	  if ((std::numeric_limits<T>::digits == 32))
		ret = _mm256_shuffle_epi8(_mm256_or_si256(acc1, arg1), io32);
	  if ((std::numeric_limits<T>::digits == 64))
		ret = _mm256_shuffle_epi8(_mm256_or_si256(acc1, arg1), io64);
	}

	_mm256_store_si256((__m256i *) (&output + idx), ret);
	//output = _mm256_extract
	//	std::memcpy(&output[idx], &ret, 32);
  }

  /**
   * class flipAVXArray
   *
   * \brief function with the fastest manipulation on 8bit integer
   * \note Compiler must be optimised with avx (either -mavx or -O3)
   * \16 bit version is slow as it uses __builtin_bswap16 and std:copy
   * \author  Anthony Campbell
   * \version $Revision: 1.0 $
   * \date $Date: 2017/10/24 14:16:20 $
   * Contact: anthony\@claydonkey.com
   * Created on: Tues October 24 18:39:37 2017
   * $Id: flipAVXArray.html,v 1.5 2017/10/24 14:16:20 bv Exp $
   * @param a: an array og type T and size N
   * Side effects - writes new values to array:a
   *
   */
  template <typename T, size_t N>
  void flipAVXArray(T(&a)[N]) {
	if ((std::numeric_limits<T>::digits == 16)) {
	  _bitflipbyte(reinterpret_cast<uint8_t*> (a),
			  static_cast<uint8_t> (ceil((N * 2) / 32.0)), bits::k1);
	  uint16_t b[N];
	  for (int i = 0; i < N; i++)
		b[i] = __builtin_bswap16(a[i]);
	  std::copy(std::begin(b), std::end(b), std::begin(a));
	}

	if ((std::numeric_limits<T>::digits == 8))
	  _bitflipbyte(reinterpret_cast<uint8_t*> (a),
			static_cast<uint8_t> (ceil(N / 32.0)), bits::k1);
  }

  template<class T>
  void print256(__m256i var) {
	T *val = reinterpret_cast<T*> (& var);
	constexpr uint8_t size = sizeof ( T);
	constexpr uint8_t size8 = sizeof ( T) * 8;

	for (uint64_t i = 0; i < (32 / size); i++)
	  std::cout << std::bitset < size8 > (val[i]) << std::endl;
  }

  template<class T>
  void i256toa(__m256i var, T * a) {
	T * val = reinterpret_cast<T *> (& var);
	constexpr uint8_t size = sizeof ( T);
	constexpr uint8_t size8 = sizeof ( T) * 8;
	for (uint8_t i = 0; i < (32 / size); i++) {

	  a[i] = static_cast<T> (var[i] << size8);
	}
  }

  class NullBuffer : public std::streambuf {
  public:

	int overflow(int c) {

	  return c;
	}
  };

  typedef const std::pair<uint64_t, uint64_t> range_t;

  /**
   * class AlignedVector
   *
   * \brief Class that emulates the std::vector but with 32 bit alignment
   * using composition over inheritance
   * \note Compiler must be optimised with avx (either -mavx or -O3)
   * \author  Anthony Campbell
   * \version $Revision: 1.0 $
   * \date $Date: 2017/10/24 14:16:20 $
   * Contact: anthony\@claydonkey.com
   * Created on: Tues October 24 18:39:37 2017
   * $Id: AlignedVector.html,v 1.5 2017/10/24 14:16:20 bv Exp $
   */
  template<class T>
  class AlignedVector {
  public:

	std::vector<T> vec;

	explicit AlignedVector(const std::initializer_list<T> &l) {
	  for (auto itm : l) {

		__attribute__ ((aligned(32))) T ai = itm;
		vec.push_back(ai);
	  }
	}

	explicit AlignedVector(const std::vector<T> &v) {
	  for (auto itm : v) {

		__attribute__ ((aligned(32))) T ai = itm;
		vec.push_back(ai);
	  }
	}

	template< size_t N>
	explicit AlignedVector(const T(&array)[N]) {
	  __attribute__ ((aligned(32))) T alignarr[N];

	  for (int i = 0; i < N; i++)
		alignarr[i] = array[i];
	  vec = std::vector<T> (std::begin(alignarr), std::end(alignarr));
	}

	T& operator[](int x) {

	  return vec[x];
	}
  };

  /**
   * class AVX
   *
   * \brief    auto flip = bits::AVX<uint64_t>(range, mH, mF);
   *  std::cout << flip;
   * Thie class wraps up all the bitflipping routines that are either executed through assembly or through intel intrinsics.
   * \note Compiler must be optimised with avx (either -mavx or -O3)
   * \author  Anthony Campbell
   * \version $Revision: 1.0 $
   * \date $Date: 2017/10/24 14:16:20 $
   * Contact: anthony\@claydonkey.com
   * Created on: Tues October 24 18:39:37 2017
   * $Id: AVX.html,v 1.5 2017/10/24 14:16:20 bv Exp $
   */
  template<class T>
  class AVX {
  public:

	struct DeleteAligned {

	  void operator()(T data[]) const {

		_aligned_free(data);
	  }
	};
	using aligned_unique_ptr = std::unique_ptr<T[], DeleteAligned>;
	///  Enum type of debug header.

	/** Enum type of debug header.
	 */
	enum displayFlags {
	  NONE = 0,
	  VARIABLES = 1,
	  INPUTLIST = 2
	};

	aligned_unique_ptr AllocateAligned(int alignment, uint64_t kSize) {
	#if defined (__MINGW64__)

	  __attribute__ ((aligned(32)))T * raw =
			  static_cast<T*> (_aligned_malloc(sizeof (T) * kSize, 32));
	#endif
	  return aligned_unique_ptr{raw};
	}

  private:
	static constexpr uint64_t kIntSize = sizeof (T);
	static constexpr uint8_t kIntW = std::numeric_limits<T>::digits10 + 1;
	const range_t kRange;
	const uint64_t kLength;
	const uint64_t kStep;
	const uint64_t kSize;
	bool initialized_ = false;
	aligned_unique_ptr input_;
	aligned_unique_ptr output_;
	bool align_alloc_ = false;
	const std::vector<T> in_vec_;
	std::vector<T> out_vec_;
	/// debug Header

	void outputHeader(uint8_t mH) {
	  if (VARIABLES & mH) {
		std::cout
				<< "-----------------Variables for m256i-------------------"
				<< "\n --- Quantized Size       : " <<
				kSize
				<< "\n --- Step                 : " <<
				+kStep
				<< "\n --- Length               : " <<
				+kLength
				<< "\n --- Integer              : " <<
				+kIntSize * 8 << " bits" << std::endl;
		std::cout
				<< "-----------------------------------------------------" <<
				std::endl;
	  }

	  if (this->in_vec_.empty()) {
		if (INPUTLIST & mH) {
		  for (T i = 0, c = 0; i < kLength; i++, c++) {
			std::cout << std::setfill('0') <<
					std::setw(kIntW) <<
					+input_[c] << " ";
			if (!((i + 1) % (24 / kIntSize)) && i != 0) std::cout << std::endl;
		  }
		}
	  }
	  else {
		if (INPUTLIST & mH) {
		  uint64_t i = 0;
		  for (auto ele : in_vec_) {
			std::cout << std::setfill('0') << std::setw(kIntW) << +ele << " ";
			if (!((i + 1) % (24 / kIntSize)) && i != 0) std::cout << std::endl;
			i++;
		  }
		}
	  }

	  if (INPUTLIST & mH) std::cout << std::endl;
	}

  public:
	char mFormat = 'B';
	template<class S>
	friend std::ostream & operator<<(std::ostream& s, bits::AVX<S> & p) noexcept;

	AVX(AVX const &) = delete;
	AVX & operator=(AVX const &) = delete;

	AVX & operator=(AVX &&rhs) {
	  if (this != &rhs)
		output_ = std::move(rhs.output_);

	  return *this;
	}

	AVX(AVX &&rhs) :
	kLength(rhs.kLength),
	kStep(rhs.kStep),
	kSize(rhs.kSize),
	output_(std::move(rhs.output_)) {
	}

	/// Create an AVX from an array of T integers

	/**
	 *  Initialize from array of type T.
	 *  @param N size of array
	 *  @param array: array of kSize N: <b>Stored in array input_ of type T and  kSize = (kStep * ceill(N / kStep) </b>
	 *  @param mF Integer format flag: <b> 'b' binary,  'd' decimal, 'n' remove trailing bits. Capitalized adds a carriage return after element</b>
	 * @param  mH Debug header enum flag: <b>NONE, INPUTLIST, VARIABLES</b>
	 *
	 */
	template< size_t N = 0 >
	explicit AVX(const T(&a)[N], uint8_t mH = 0, char mF = 'B') :
	mFormat(mF),
	kLength(N),
	kStep(32 / kIntSize),
	kSize(kStep * ceil(static_cast<double> (N) / kStep)),
	align_alloc_(true),
	out_vec_(kSize) {
	  input_ = AllocateAligned(32, kSize);
	  output_ = AllocateAligned(32, kSize);

	  memset(input_.get(), std::numeric_limits<int>::max(), kIntSize * kSize);

	  for (uint64_t i = 0; i < N; i++)
		input_[i] = a[i];
	  outputHeader(mH);
	  initialized_ = true;
	}

	/// Create an AVX from an aligned vector of T integers

	/**
	 *  Initialize from aligned vector of type T.
	 *  @param aligned vector: array of kSize N:
	 * <b>Stored in array input_ of type T and  kSize = (kStep * ceill(N / kStep) </b>
	 *  @param mF Integer format flag: <b> 'b' binary,  'd' decimal,
	 * 'n' remove trailing bits. Capitalized adds a carriage return after element</b>
	 * @param  mH Debug header enum flag: <b>NONE, INPUTLIST, VARIABLES</b>
	 *
	 */
	explicit AVX(const bits::AlignedVector<T> & avec,
			uint8_t mH = 0, char mF = 'B') :
	mFormat(mF),
	kStep(32 / kIntSize),
	kSize(kStep * ceil(static_cast<double> (avec.vec.size()) / kStep)),
	in_vec_(avec.vec),
	kLength(avec.vec.size()),
	align_alloc_(true),
	out_vec_(kSize) {
	  input_ = AllocateAligned(32, kSize);
	  output_ = AllocateAligned(32, kSize);
	  uint64_t i = 0;
	  for (auto val : in_vec_) {

		input_[i] = val;
		i++;
	  }
	  outputHeader(mH);
	  initialized_ = true;
	}

	/// Create an AVX from an array of T integers

	/**
	 *  Initialize from vector  of type T.
	 *  @param vec: the vector
	 *  @param array: array of kSize N:
	 *  <b>Stored in array input_ of type T and  kSize = (kStep * ceill(N / kStep) </b>
	 *  @param mF Integer format flag:
	 *  <b> 'b' binary
	 *  'd' decimal
	 *  'n' remove trailing bits
	 *  Capitalized adds a carriage return after element</b>
	 * @param  mH Debug header enum flag: <b>NONE, INPUTLIST, VARIABLES</b>
	 *
	 */
	explicit AVX(const std::vector<T> & vec, uint8_t mH = 0, char mF = 'B') :
	mFormat(mF),
	kStep(32 / kIntSize),
	kSize(kStep * ceil(reinterpret_cast<double> (vec.size()) / kStep)),
	in_vec_(vec),
	kLength(vec.size()),
	align_alloc_(true),
	out_vec_(kSize) {
	  input_ = AllocateAligned(32, kSize);
	  output_ = AllocateAligned(32, kSize);
	  uint64_t i = 0;
	  for (auto val : in_vec_) {

		input_[i] = val;
		i++;
	  }
	  outputHeader(mH);
	  initialized_ = true;
	}

	/// Create an AVX from a range of uint64_t integers

	/**
	 *  Initialise from range.
	 *  @param range: array of size 2: <b>range[0] = lower limit, range[1] = upper limit</b>
	 *  @param mF Integer format flag:
	 * <b> 'b' binary
	 * 'd' decimal
	 * 'v' decimal + remove trailing bits
	 * 'w' binary remove trailing bits
	 * 'h' hexadecimal
	 *  captalised character flag adds a carriage return after each element</b>
	 * @param  mH Debug header enum flag: <b>NONE, INPUTLIST, VARIABLES</b>
	 *
	 */
	explicit AVX(const range_t range, uint8_t mH = 0, char mF = 'B') :
	mFormat(mF),
	kRange(range),
	kLength(range.second - range.first + 1),
	kStep(32 / kIntSize),
	kSize(kStep * ceil(static_cast<double> (kLength) / kStep)),
	align_alloc_(true),
	out_vec_(kSize) {
	  uint64_t big = kRange.first > kRange.second ?
			  kRange.first : kRange.second;
	  uint8_t mBytes = (big >= 0xFFFFFFFF) ? 8 :
			  (big >= 0xFFFF) ? 4 : (big >= 0xFF) ? 2 : 1;

	  if (mBytes > kIntSize) {
		std::cout << "Range Integer too large for AVX Type" << std::endl;
		initialized_ = false;
		return;
	  }
	  input_ = AllocateAligned(32, kSize);
	  output_ = AllocateAligned(32, kSize);
	  for (uint64_t i = kRange.first, c = 0; i <= kRange.second; i++, c++)
		input_[c] = i;
	  outputHeader(mH);
	  initialized_ = true;
	}

	/// Return vector of flipped integers
	/**
	 *  flip vector  of integers and return a vector  of the flipped integers
	 * @return   std::vector<T> : vector  of integer type T
	 * flipAVX takes in 256bits at a time
	 **/
	auto vFlip()noexcept -> std::vector<T> {
	  if (initialized_) {
		for (uint64_t j = 0; j < kSize; j += kStep) {
		  __m256i r = bits::flipAVX(&input_[ j]);
		  _mm256_storeu_si256((__m256i *) (&out_vec_[j]), r);
		  /* unaligned for std::vector*/
		//  std::memcpy(&out_vec_[j], &r, 32);
		}
	  }
	  return out_vec_;
	}
	/// Return pointer to flipped integers
	/**
	 *  flip array of integers and return pointer of the flipped array
	 * @return   aligned_unique_ptr<T> : pointer of array of integer type T
	 * @note The output array is in blocks of 256 bits.
	 * flipAVX takes in 256bits at a time
	 * There will be null values present for <b><> kLength  * TS  % 32  and << kLength  * TS  % 32 </b>
	 * where:
	 * TS is Integer byte length
	 *  kLength is the number of elements in the input array
	 **/
	auto unique_pFlip() noexcept -> aligned_unique_ptr {
	  if (initialized_) {
		for (uint64_t j = 0; j < kSize; j += kStep) {
		  __m256i r = bits::flipAVX(&input_[j]);
		  _mm256_store_si256((__m256i *) (&output_[j]), r);
		  //  std::memcpy(&output_[j], &r, 32);
		}
	  }
	  return std::move(output_);
	}
	/// Return pointer to flipped integers
	/**
	 *  flip array of integers and return pointer of the flipped array
	 * @return   std::shared_ptr<T[] > : pointer of array of integer type T
	 * @note The output array is in blocks of 256 bits.
	 * flipAVX takes in 256bits at a time
	 * There will be null values present for <b><> kLength  * TS  % 32  and << kLength  * TS  % 32 </b>
	 * where:
	 * TS is Integer byte length
	 *  kLength is the number of elements in the input array
	 **/
	auto shared_pFlip()noexcept -> std::shared_ptr < T[] > {
	  if (initialized_) {
		for (uint64_t j = 0; j < kSize; j += kStep) {
		  __m256i r = bits::flipAVX(&input_[ j]);
		  _mm256_store_si256((__m256i *) (&output_[j]), r);
		  //  std::memcpy(&output_[j], &r, 32);
		}
	  }

	  return std::shared_ptr < T[] >(output_.get());
	}
	/// Return pointer to flipped integers

	/**
	 *  flip array of integers and return pointer of the flipped array
	 * @return   T * : pointer of array of integer type T
	 * @note Flying into the Danger Zone. Must declare a aligned free function to release pointer.
	 * The output array is in blocks of 256 bits.
	 * flipAVX takes in 256bits at a time
	 * There will be null values present for <b><> kLength  * TS  % 32  and << kLength  * TS  % 32 </b>
	 * where:
	 * TS is Integer byte length
	 *  kLength is the number of elements in the input array
	 **/
	T * pFlip() noexcept {
	  if (initialized_) {
		for (uint64_t j = 0; j < kSize; j += kStep) {
		  //  bits::flippAVX(&input_[ j], output_, j);
		  __m256i r = bits::flipAVX(&input_[ j]);
		  _mm256_store_si256((__m256i *) (&output_[j]), r);
		  //  std::memcpy(&output_ [j], &r, 32);
		}
	  }
	  return output_.get();
	}
	#include <array>
	//	#include <condition_variable>
  };
  /// stream of the array of AVX vectors

  /**
   * @param lhs ostream
   * @return ostream
   * The output stream is a block of 32 aligned values.
   * .There will return null values for < > kLength  * TS  % 32  and < kLength  * TS  % 32
   * where:
   * TS is Integer byte length
   *  kLength is the number of elements in the input array
   **/
  template<class T >
  std::ostream & operator<<(std::ostream& lhs, bits::AVX<T>& p) noexcept {
	if (p.initialized_) {
	  constexpr uint8_t intW = std::numeric_limits<T>::digits10 + 1;
	  constexpr uint8_t intH = std::numeric_limits<T>::digits + 1;

	  for (uint64_t j = 0; j < p.kSize; j += p.kStep) {
		__m256i m = bits::flipAVX(&p.input_[ j]);
		auto _p_var(reinterpret_cast<T*> (& m));
		for (uint8_t i = 0; i < p.kStep; i++) {
		  switch (tolower(p.mFormat)) {
			case 'd':
			  lhs <<
					  std::setfill('0') <<
					  std::setw(intW) <<
					  std::dec <<
					  +_p_var[i];
			  break;
			case 'b':
			  lhs <<
					  std::bitset < p.kIntSize * 8 > (_p_var[i]);
			  break;
			case 'v':
			  lhs << (_p_var[i] >> __builtin_ctzll(_p_var[i]));
			  break;
			case 'h':
			  lhs <<
					  std::setfill('0') <<
					  std::setw(16) <<
					  std::hex <<
					  +p.input_[i + j] <<
					  "->" << std::hex << _p_var[i];
			  break;
			case'w':
			  lhs <<
					  std::bitset < p. kIntSize * 8 > (_p_var[i]
					  >> __builtin_ctzll(_p_var[i]));
			  break;
			case 'i':
			  // std::setfill('0') << std::setw(intW) <<
			  lhs <<
					  std::dec <<
					  +p.input_[i + j] <<
					  "-->" <<
					  +(_p_var[i] >> __builtin_ctzll(_p_var[i]));
			  break;
		  }
		  isupper(p.mFormat) ? lhs << std::endl : lhs << " ";
		}
	  }
	}
	return lhs;
  }
} // namespace bits
  #endif
#endif  // INCLUDE_BITFLIPAVX_H_

