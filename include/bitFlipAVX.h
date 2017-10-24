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
  #include <cstring>
  #include <cstdlib>
  #include <iostream>
  #include <iomanip>
  #include <bitset>
  #include <limits>
  #include<immintrin.h>
  #include <tuple>
  #include <vector>

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
	0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F,
	0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E, 0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F, 0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E, 0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F,
	0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0, 0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0, 0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0, 0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0
  };


  __attribute__ ((aligned(32))) static const uint8_t a1[32] = {
	0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F
  };
  __attribute__ ((aligned(32))) static const uint8_t a2[32] = {
	0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E, 0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F, 0X00, 0X08, 0X04, 0X0C, 0X02, 0X0A, 0X06, 0X0E, 0X01, 0X09, 0X05, 0X0D, 0X03, 0X0B, 0X07, 0X0F
  };
  __attribute__ ((aligned(32))) static const uint8_t a3[32] = {
	0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0, 0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0, 0X00, 0X80, 0X40, 0XC0, 0X20, 0XA0, 0X60, 0XE0, 0X10, 0X90, 0X50, 0XD0, 0X30, 0XB0, 0X70, 0XF0
  };

  __attribute__ ((aligned(32))) static const uint8_t o256[32] = {
	31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
  };

  __attribute__ ((aligned(32))) static const uint8_t o16[32] = {
	1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14, 17, 16, 19, 18, 21, 20, 23, 22, 25, 24, 27, 26, 29, 28, 31, 30
  };

  __attribute__ ((aligned(32))) static const uint8_t o32[32] = {
	3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12, 19, 18, 17, 16, 23, 22, 21, 20, 27, 26, 25, 24, 31, 30, 29, 28
  };

  __attribute__ ((aligned(32))) static const uint8_t o64[32] = {
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
   *   8 * 32
   *  16 * 16
   * 32 * 32
   */

  typedef struct ZCntFlip {
	__m256i _Flip;
	long long _zCnt[4];
  } __ZCntFlip;

  template<class T >
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
	for (uint64_t i = 0; i < (32 / size); i++)
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

  typedef const std::pair<uint64_t, uint64_t> range_t;
  /// class AVX

  /**
   * class AVX
   *
   * \ingroup bits
   * (Note, this needs exactly one \defgroup somewhere)
   * \brief    auto flip = bits::AVX<uint64_t>(range, mH, mF);
   *  std::cout << flip;
   * Thie class wraps up all the bitflipping routines that are either executed through assembly or through intel intrinsics.
   * \note Compiler must be optimised with avx (either -mavx or -O3)
   * \author  Anthony Campbell
   * \version $Revision: 1.0 $
   * \date $Date: 2017/10/24 14:16:20 $
   * Contact: anthony\@claydonkey.com
   * Created on: Tues October 24 18:39:37 2017
   * $Id: AVX-Flipbits.html,v 1.5 2017/10/24 14:16:20 bv Exp $
   */
  template<class T>
  class AVX {
  public:

	///  Enum type of debug header.

	/** Enum type of debug header.
	 */
	enum displayFlags {
	  NONE = 0,
	  VARIABLES = 1,
	  INPUTLIST = 2
	};
  private:

	static constexpr uint64_t _IntSize = sizeof (T);
	static constexpr uint8_t intW = std::numeric_limits<T>::digits10 + 1;
	const range_t _Range;
	const uint64_t _Length;
	const uint64_t _Step;
	const uint64_t _Size;
	bool Initialized = false;
	T * pVar;
	__attribute__ ((aligned(32))) T * _Input;
	__attribute__ ((aligned(32))) T *_Output;
	bool _AlignAlloc = false;
	const std::vector<T> _InVec;
	///debug Header

	void outputHeader(uint8_t mH) {

	  if (VARIABLES & mH) {
		std::cout
				<< "------------------------------------Variables for m256i-------------------------------------"
				<< "\n --- Quantized Size       : " << _Size
				<< "\n --- Step                 : " << +_Step
				<< "\n --- Length               : " << +_Length
				<< "\n --- Integer              : " << +_IntSize * 8 << " bits" << std::endl;
		std::cout
				<< "--------------------------------------------------------------------------------------------" << std::endl;
	  }
	  if (this->_InVec.empty()) {
		if (INPUTLIST & mH) {
		  for (T i = 0, c = 0; i < _Length; i++, c++) {
			std::cout << std::setfill('0') << std::setw(intW) << +_Input[c] << " ";
			if (!((i + 1) % (24 / _IntSize)) && i != 0) std::cout << std::endl;
		  }
		}
	  }
	  else {
		if (INPUTLIST & mH) {
		  uint64_t i = 0;
		  for (auto ele : _InVec) {
			std::cout << std::setfill('0') << std::setw(intW) << +ele << " ";
			if (!((i + 1) % (24 / _IntSize)) && i != 0) std::cout << std::endl;
			i++;
		  }
		}
	  }
	  if (INPUTLIST & mH) std::cout << std::endl;
	}
  public:

	char mFormat = 'B';
	template<class S>
	friend std::ostream & operator<<(std::ostream& s, bits::AVX<S> & p);

	/// Create an AVX from an array of T integers

	/**
	 *  Initialize from array of type T.

	 *  @param N size of array
	 *  @param array: array of _Size N: <b>Stored in array _Input of type T and  _Size = (_Step * ceill(N / _Step) </b>
	 *  @param mF Integer format flag: <b> 'b' binary,  'd' decimal, 'n' remove trailing bits. Capitalized adds a carriage return after element</b>
	 * @param  mH Debug header enum flag: <b>NONE, INPUTLIST, VARIABLES</b>
	 *
	 */
	template< size_t N = 0 >
	AVX(const T(&a)[N], uint8_t mH = 0, char mF = 'B') :
	mFormat(mF),
	_Length(N),
	_Step(32 / _IntSize),
	_Size(_Step * ceil((double) N / _Step)),
	_AlignAlloc(true) {

	#if defined (__MINGW64__)
	  _Input = (T *) _aligned_malloc(_IntSize * _Size, 32);
	  _Output = (T *) _aligned_malloc(_IntSize * _Size, 32);
	#else
	  _Input = (T *) _aligned_malloc(_IntSize * _Size), 32);
	  _Output = (T *) _aligned_malloc(_IntSize * _Size, 32);
	#endif

	  memset(_Input, std::numeric_limits<int>::max(), _IntSize * _Size);
	  for (uint64_t i = 0; i < N; i++)
		_Input[i] = a[i];
	  outputHeader(mH);
	  Initialized = true;
	}

	/// Create an AVX from an array of T integers

	/**
	 *  Initialize from vector  of type T.

	 *  @param vec: the vector
	 *  @param array: array of _Size N: <b>Stored in array _Input of type T and  _Size = (_Step * ceill(N / _Step) </b>
	 *  @param mF Integer format flag: <b> 'b' binary,  'd' decimal, 'n' remove trailing bits. Capitalized adds a carriage return after element</b>
	 * @param  mH Debug header enum flag: <b>NONE, INPUTLIST, VARIABLES</b>
	 *
	 */

	AVX(const std::vector<T> vec, uint8_t mH = 0, char mF = 'B') :
	mFormat(mF),
	_Step(32 / _IntSize),
	_Size(_Step * ceil((double) vec.size() / _Step)),
	_InVec(vec),
	_Length(vec.size()),
	_AlignAlloc(true) {

	#if defined (__MINGW64__)
	  _Input = static_cast<T*> (_aligned_malloc(_IntSize * _Size, 32));
	  _Output = static_cast<T*> (_aligned_malloc(_IntSize * _Size, 32));
	#else
	  _Input = static_cast<T*> (_aligned_malloc(_IntSize * _Size), 32);
	  _Output = static_cast<T*> (_aligned_malloc(_IntSize * _Size, 32));
	#endif

	  uint64_t i = 0;
	  for (auto val : _InVec) {
		_Input[i] = val;
		i++;
	  }
	  //  memcpy(&_Input, &_InVec, _IntSize * _Size);

	  outputHeader(mH);
	  Initialized = true;
	}

	/// Create an AVX from a range of uint64_t integers

	/**
	 *  Initialise from range.
	 *  @param range: array of size 2: <b>range[0] = lower limit, range[1] = upper limit</b>
	 *  @param mF Integer format flag: <b> 'b' binary,  'd' decimal, 'v' decimal + remove trailing bits 'w' binary remove trailing bits, 'h' hexadecimal.
	 *  captalised character flag adds a carriage return after each element</b>
	 * @param  mH Debug header enum flag: <b>NONE, INPUTLIST, VARIABLES</b>
	 *
	 */
	AVX(const range_t range, uint8_t mH = 0, char mF = 'B') :
	mFormat(mF),
	_Range(range),
	_Length(range.second - range.first + 1),
	_Step(32 / _IntSize),
	_Size(_Step * ceill((double) (_Length) / _Step)),
	_AlignAlloc(true) {

	  uint64_t big = _Range.first > _Range.second ? _Range.first : _Range.second;
	  uint8_t mBytes = (big >= 0xFFFFFFFF) ? 8 : (big >= 0xFFFF) ? 4 : (big >= 0xFF) ? 2 : 1;

	  if (mBytes > _IntSize) {
		std::cout << "Range Integer too large for AVX Type" << std::endl;
		Initialized = false;
		return;
	  }
	#if defined (__MINGW64__)
	  _Input = static_cast<T*> (_aligned_malloc(_IntSize * _Size, 32));
	  _Output = static_cast<T*> (_aligned_malloc(_IntSize * _Size, 32));
	#else
	  _Input = static_cast<T*> (_aligned_malloc(_IntSize * _Size), 32);
	  _Output = static_cast<T*> (_aligned_malloc(_IntSize * _Size, 32));
	#endif

	  memset(_Input, std::numeric_limits<int>::max(), _IntSize * _Size);
	  for (uint64_t i = _Range.first, c = 0; i <= _Range.second; i++, c++)
		_Input[c] = i;

	  outputHeader(mH);
	  Initialized = true;
	}

	~AVX() {
	  if (_AlignAlloc) {
	#if defined (__MINGW64__)
		_aligned_free(_Input);
		_aligned_free(_Output);
		_Input = NULL;
	#else
		_aligned_free(_Input);
		_aligned_free(_Output);
		_Input = NULL;
	#endif
	  }
	}

	const std::vector<T> flipVec() {
	  if (Initialized && _InVec.empty()) {
		for (uint64_t j = 0; j < _Size; j += _Step) {
		  __m256i m = bits::flipAVX(_Input + j);
		  for (uint64_t i = 0; i < _Step; i++)
			_Output[ j + i] = (T) (m[i]);
		}
		return std::vector<T> (_Output, _Output + sizeof (_Output) / _IntSize);
	  }
	  else
		if (Initialized) {
		std::vector<T> out(_Size);
		for (uint64_t j = 0; j < _Size; j += _Step) {
		  T x[_Step];
		  memcpy(&x, &_InVec[j], 32);
		  __m256i m = bits::flipAVX(x);
		  memcpy(&out[j], &m, 32);
		}
		return out;
	  }
	}

	/// Return pointer to flipped integer

	/**
	 *  flip array of integers and return pointer of the flipped array
	 * @return   T * : pointer of array of integer type T
	 * @note The output array is in blocks of 256 bits.
	 * flipAVX takes in 256bits at a time
	 * There will be null values present for <b><> _Length  * TS  % 32  and << _Length  * TS  % 32 </b>
	 * where:
	 * TS is Integer byte length
	 *  _Length is the number of elements in the input array
	 **/

	T * flip() {
	  if (Initialized && _InVec.empty()) {
		for (uint64_t j = 0; j < _Size; j += _Step) {
		  __m256i m = bits::flipAVX(_Input + j);
		  for (uint64_t i = 0; i < _Step; i++)
			_Output[ j + i] = (T) (m[i]);
		}
	  }
	  return _Output;
	}
	#include <array>


	//	std::array<T, _Size > & flip2() {
	//	  if (Initialized) {
	//
	//		for (uint64_t j = 0; j < _Size; j += _Step) {
	//		  __m256i m = bits::flipAVX(_Input + j);
	//		  pVar = reinterpret_cast<T *> (& m);
	//		  for (uint64_t i = 0; i < _Step; i++)
	//			_Output[ j + i] = pVar[i];
	//		}
	//	  }
	//
	//	  return *reinterpret_cast<std::array<T, _Size>*> (&_Output);
	//	}

	template <size_t N = 0 >
	static void flipAVXArray(T(&a)[N]) {
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
  };
  /// stream of the array of AVX vectors

  /**
   * @param lhs ostream
   * @return ostream
   * The output stream is a block of 32 aligned values.
   * .There will return null values for < > _Length  * TS  % 32  and < _Length  * TS  % 32
   * where:
   * TS is Integer byte length
   *  _Length is the number of elements in the input array
   **/
  template<class T >
  std::ostream & operator<<(std::ostream& lhs, bits::AVX<T>& p) {
	if (p.Initialized) {

	  constexpr uint8_t intW = std::numeric_limits<T>::digits10 + 1;
	  constexpr uint8_t intH = std::numeric_limits<T>::digits + 1;

	  for (uint64_t j = 0; j < p._Size; j += p._Step) {//p._Size; j += p._Step) {
		__m256i m = bits::flipAVX(p._Input + j);
		p.pVar = (T *) (& m);
		for (uint8_t i = 0; i < p._Step; i++) {
		  switch (tolower(p.mFormat)) {
			case 'd':
			  lhs << std::setfill('0') << std::setw(intW) << std::dec << +p.pVar[i];
			  break;
			case 'b':
			  lhs << std::bitset < p._IntSize * 8 > (p.pVar[i]);
			  break;
			case 'v':
			  lhs << (p.pVar[i] >> __builtin_ctzll(p.pVar[i]));
			  break;
			case 'h':
			  lhs << std::setfill('0') << std::setw(16) << std::hex << +p._Input[i + j] << "->" << std::hex << p.pVar[i];
			  break;
			case'w':
			  lhs << std::bitset < p. _IntSize * 8 > (p.pVar[i] >> __builtin_ctzll(p.pVar[i]));
			  break;
			case 'i':
			  lhs << std::setfill('0') << std::setw(intW) << std::dec << +p._Input[i + j] << "->" << std::setfill('0') << std::setw(intW) << +(p.pVar[i] >> __builtin_ctzll(p.pVar[i]));
			  break;
		  }
		  isupper(p.mFormat) ? lhs << std::endl : lhs << " ";
		}
	  }
	}
	return lhs;
  }
}

  #endif
#endif /* BITREVERSEAVX_H */

