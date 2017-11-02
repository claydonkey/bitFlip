/*
 * File:   bitFliptestsuite
 * Author: Anthony Campbell
 *
 * Created on 06-Oct-2017, 16:08:35
 */

#include <gtest/gtest.h>
#ifndef NOBENCHMARK
#include <benchmark/benchmark.h>
#endif

#include <stdint.h>
#include <bitset>
#include <math.h>
#include <omp.h>
#include <iostream>
#include <cmath>
#include <time.h>
#include "bitFlip.h"
#include "bitFlipAVX.h"
#include "bitFlipTables.h"
#include "typeDeduce.h"
#include "bitFlipTest.h"

using namespace std;
using namespace bits;

#define ARRAY_SIZE 10240
uint16_t i;
uint16_t rev1, rev2;
uint8_t hex8, rhex8;
uint16_t hex16, rhex16;
uint32_t hex32, rhex32;
uint64_t hex64, rhex64;
uint16_t cntbits;
uint8_t foo2[ARRAY_SIZE];
uint8_t foo3[ARRAY_SIZE];
uint8_t foo8[ARRAY_SIZE];
uint16_t foo16[ARRAY_SIZE / 2];
uint32_t foo32[ARRAY_SIZE / 4];
uint64_t foo64[ARRAY_SIZE / 8];
uint16_t foo16T[0xFFFF + 1];

__attribute__ ((aligned(32))) uint8_t fooZ8[ARRAY_SIZE + 32] = {};
__attribute__ ((aligned(32))) uint16_t fooZ16[(ARRAY_SIZE / 2) + 16] = {};
__attribute__ ((aligned(32))) uint32_t fooZ32[(ARRAY_SIZE / 4) + 8] = {};
__attribute__ ((aligned(32))) uint64_t fooZ64[(ARRAY_SIZE / 8) + 4] = {};

__m256i flipin[ARRAY_SIZE / 32];
__m256i flipout[ARRAY_SIZE / 32];

// Constants (first we got the mask, then the high order nibble look up table and last we got the low order nibble lookup table)

void setup() {

  srand(time(NULL));

  for (uint32_t i = 0; i < ARRAY_SIZE; i++) {
	foo8[i] = fooZ8[i] = foo3[i] = foo2[i] = rand();

	if (!(i % 8) & !i == 0) {
	  flipin[i / 4 / 8][(i / 8) % 4] = fooZ64[(i / 8)] = foo64[(i / 8)] = ((uint64_t) foo8[i - 8]) + ((uint64_t) foo8[i - 7] << 8) + \
                ((uint64_t) foo8[i - 6] << 16) + ((uint64_t) foo8[i - 5] << 24) + ((uint64_t) foo8[i - 4] << 32) + \
                ((uint64_t) foo8[i - 3] << 40) +((uint64_t) foo8[i - 2] << 48) + ((uint64_t) foo8[i - 1] << 56);
	}

	if (!(i % 4) & !i == 0) {
	  fooZ32[(i / 4)] = foo32[(i / 4)] = ((uint64_t) foo8[i - 4]) + ((uint64_t) foo8[i - 3] << 8) + ((uint64_t) foo8[i - 2] << 16) + ((uint64_t) foo8[i - 1] << 24);
	}

	if (!(i % 2) & !i == 0) {
	  fooZ16[(i / 2)] = foo16[(i / 2)] = ((uint64_t) foo8[i - 2]) + ((uint64_t) foo8[i - 1] << 8);
	}
  }

  fooZ8[0] = foo8[0] = foo3[0] = foo2[0] = hex8 = rand();
  rhex8 = flipNaive<uint8_t>(hex8);
  fooZ16[0] = foo16[0] = hex16 = rand();
  rhex16 = flipNaive<uint16_t>(hex16);
  fooZ32[0] = foo32[0] = hex32 = ((uint32_t) rand() << 16) | rand();
  rhex32 = flipNaive<uint32_t>(hex32);
  flipin[0][0] = fooZ64[0] = foo64[0] = hex64 = ((uint64_t) rand() << 32) | rand();
  rhex64 = flipNaive<uint64_t>(hex64);
#ifndef TABLEHEADERS
  //Generate 8bit table
  for (uint16_t i = 0; i <= 0xFF; i++) {
	bitFlipTable8[i] = flipMask<uint8_t>((uint8_t&) i);
#ifdef PRINTTABLE8
	cout << "0x" << std::hex << bitFlipTable8[i] << ", ";
	if (!(i % 16)) cout << "\\" << endl;
#endif
  }
  //Generate 1bit table
  for (uint64_t i = 0x00; i <= 0xFFFF; i++) {
	uint8_t i2 = static_cast<uint8_t> (i >> 8);
	uint16_t BRH = static_cast<uint16_t> ((flipMask<uint8_t>(static_cast<uint8_t&> (i2)) & 0xff));
	uint16_t BRL = static_cast<uint16_t> (flipMask<uint8_t>((uint8_t&) i)) << 8;
	uint16_t BR = BRL + BRH;
	bitFlipTable16[i] = BR;

	//   cout << bitset<16>(bitFlipTable16[i]) << endl;
#ifdef PRINTTABLE16
	cout << "0x" << std::hex << bitFlipTable16[i] << ", ";
	if (!(i % 16)) cout << "\\" << endl;
#endif
  }
#endif
}

class TestSuite : public testing::Test {
public:

  void SetUp() {
	setup();
  }

};
/*--------------------------------------------------      flip Counters      --------------------------------------------------*/

/*------------------------------------------------------ Begin BENCHMARKS -----------------------------------------------------*/
#ifndef NOBENCHMARK
#define BUFFERCNT ARRAY_SIZE/8
#define INTTYPE uint64_t

void BM_Flip_IntrAVX64(benchmark::State& state) {

  constexpr uint16_t intsize = sizeof (INTTYPE);
  constexpr uint16_t arrsize = 32 / intsize;
  while (state.KeepRunning()) {
	for (INTTYPE i = 0; i < ARRAY_SIZE / 8; i += arrsize)
	  flipout[i / arrsize] = bits::flipAVX(&fooZ64[i]);
  }
}

void BM_Flip_IntrAVX64i256(benchmark::State& state) {

  constexpr uint16_t intsize = sizeof (INTTYPE);
  while (state.KeepRunning()) {
	for (INTTYPE i = 0; i < ARRAY_SIZE / 32; i += 1) {
	  flipout[i] = bits::flipAVX256(&flipin[i]);
	}
  }
}

void BM_Flip_IntrAVXClassNullBuffer(benchmark::State& state) {
  bits::AlignedVector<uint64_t> avec(foo64);
  bits::NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  bits::AVX<uint64_t> avx(avec);
  while (state.KeepRunning()) {
	null_stream << avx;
  }
}

void BM_Flip_IntrAVXClassShared_ptr(benchmark::State& state) {
  bits::AlignedVector<uint64_t> avec(foo64);
  bits::AVX<uint64_t> avx2(avec);
  while (state.KeepRunning()) {
	auto arr = avx2.shared_pFlip();
	//	_aligned_free(arr);
  }
}

void BM_Flip_IntrAVXClassUnique_ptr(benchmark::State& state) {
  bits::AlignedVector<uint64_t> avec(foo64);
  auto avec2 = bits::AlignedVector<uint32_t>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
  bits::AVX<uint64_t> avx2(avec);
  while (state.KeepRunning()) {
	//   auto arr = avx2.pFlip();
  }
}

void BM_Flip_IntrAVXArr_VecClass(benchmark::State& state) {

  bits::AVX<uint64_t> avx(fooZ64);
  while (state.KeepRunning()) {
	auto arr = avx.vFlip();
  }
}

void BM_Flip_IntrAVXVec_VecClass(benchmark::State& state) {
  bits::AlignedVector<uint64_t> avec(foo64);
  bits::AVX<uint64_t> avx(avec);
  while (state.KeepRunning()) {
	auto arr = avx.vFlip();
  }
}

void BM_Flip_AVX(benchmark::State& state) {
  while (state.KeepRunning())
	bits::flipAVXArray(fooZ8);
}

void BM_Flip_AVX16(benchmark::State& state) {
  while (state.KeepRunning())
	bits::flipAVXArray(fooZ16);
}

void BM_Flip_NaiveArrayll(benchmark::State& state) {
  while (state.KeepRunning())
	flipNaiveArrayll(foo64, ARRAY_SIZE / 8);
}

void BM_Flip_NaiveLambda(benchmark::State& state) {
  while (state.KeepRunning())
	flipNaiveLambdaArray(foo64);
}

void BM_Flip_lloop(benchmark::State& state) {
  while (state.KeepRunning())
	_bitflipllloop(foo64, ARRAY_SIZE / 8);
}

void BM_Flip_Mask(benchmark::State& state) {
  while (state.KeepRunning())
	flipMaskArray<uint8_t>(foo2);
}

void BM_Flip_Naive(benchmark::State& state) {
  while (state.KeepRunning())
	flipNaiveArray<uint8_t>(foo3);
}

void BM_Flip_Naive64(benchmark::State& state) {
  while (state.KeepRunning())
	flipNaiveArray<uint64_t>(foo64);
}

void BM_Flip_Table16(benchmark::State& state) {
  while (state.KeepRunning())
	flipTableArray<uint16_t>(foo16);
}

void BM_Flip_Table32(benchmark::State& state) {
  while (state.KeepRunning())
	flipTableArray<uint32_t>(foo32);
}


#ifndef NOASSMBLR
BENCHMARK(BM_Flip_AVX);
BENCHMARK(BM_Flip_AVX16);
#endif
BENCHMARK(BM_Flip_IntrAVX64);
BENCHMARK(BM_Flip_IntrAVX64i256);
BENCHMARK(BM_Flip_IntrAVXArr_VecClass);
BENCHMARK(BM_Flip_IntrAVXVec_VecClass);
BENCHMARK(BM_Flip_IntrAVXClassShared_ptr);
BENCHMARK(BM_Flip_IntrAVXClassUnique_ptr);
BENCHMARK(BM_Flip_IntrAVXClassNullBuffer);

BENCHMARK(BM_Flip_Table16);
BENCHMARK(BM_Flip_Table32);
BENCHMARK(BM_Flip_Naive);
BENCHMARK(BM_Flip_Naive64);
BENCHMARK(BM_Flip_Mask);
#ifndef NOASSMBLR
BENCHMARK(BM_Flip_lloop);
#endif
BENCHMARK(BM_Flip_NaiveArrayll);
BENCHMARK(BM_Flip_NaiveLambda);
#endif

/*--------------------------------------------------------------------- Begin TESTS -----------------------------------------------------------*/

TEST_F(TestSuite, testFlipNaiveArray) {
  flipNaiveArray(foo8);
  EXPECT_EQ(rhex8, foo8[0]);
}

TEST_F(TestSuite, testFlipNaive) {
  EXPECT_EQ(rhex8, flipNaive(hex8));
}

#ifndef NOAVX

TEST_F(TestSuite, testFlipIntrAVX) {
  __m256i res = bits::flipAVX(&hex64);
  EXPECT_EQ(bits::get256(&rhex64)[0], res[0]);
}

typedef uint64_t returnArray[128];
#define INTTYPE uint64_t

TEST_F(TestSuite, testFlipIntrArrayAVX) {
  constexpr uint16_t intsize = sizeof (INTTYPE);
  constexpr uint16_t arrsize = 32 / intsize;
  uint32_t val = rand() % ARRAY_SIZE / 8;

  for (INTTYPE i = 0; i < (ARRAY_SIZE / 8); i += arrsize)
	flipout[i / arrsize] = bits::flipAVX(&fooZ64[i]);

  EXPECT_EQ(flipout[val / 4][val % 4], bits::flipNaiveLambda(fooZ64[val]));
}

TEST_F(TestSuite, testFlipIntrClassAVX) {
  uint32_t val = rand() % ARRAY_SIZE / 8;
  bits::AlignedVector <uint64_t> avec(foo64);

  auto dfoo = bits::AVX<uint64_t> (fooZ64).pFlip();
  //	std::cout << "[WATCH] " << dfoo[val] << " " << bits::flipNaiveLambda(fooZ64[val]) << std::endl;
  EXPECT_EQ(bits::flipNaiveLambda(foo64[val]), dfoo[val]);
}

TEST_F(TestSuite, testFlipIntrVecClassAVX) {
  uint32_t val = rand() % ARRAY_SIZE / 8;

  bits::AlignedVector <uint64_t> avec(foo64);
  auto dfoo = bits::AVX<uint64_t> (avec).vFlip();
  //	std::cout << "[WATCH] " << dfoo[val] << " " << bits::flipNaiveLambda(fooZ64[val]) << std::endl;
  EXPECT_EQ(bits::flipNaiveLambda(foo64[val]), dfoo[val]);
}

#endif

TEST_F(TestSuite, testFlipAVX) {
  EXPECT_EQ(bits::flipNaive(foo8[0]), flipAVX8(foo8[0]));
}

/*
TEST_F(TestSuite, testFlipAVX16) {
	uint16_t foo16b[ARRAY_SIZE / 2];
	std::copy(std::begin(foo16), std::end(foo16), std::begin(foo16b));
	uint32_t rrange = rand() % ARRAY_SIZE;

	flipNaiveArray(foo16b);
	bits::flipAVXArray(fooZ16);
	EXPECT_EQ(foo16b[rrange], fooZ16[rrange]);
}
 */

#ifndef NOASSMBLR

TEST_F(TestSuite, testAVXArray1) {
  uint8_t foo8b[ARRAY_SIZE];
  std::copy(std::begin(foo8), std::end(foo8), std::begin(foo8b));
  uint8_t foo8c[ARRAY_SIZE];
  std::copy(std::begin(foo8), std::end(foo8), std::begin(foo8c));

  bits::flipAVXArray(fooZ8);

  //std::cout << "decltype(i) is " << type_name<decltype(flipNaive<uint8_t>)>() << '\n';
  //std::cout << "decltype(i) is " << type_name<decltype(foo8c)>() << '\n';

  bits::flipArray(foo8, flipNaive<uint8_t>);
  bits::flipArray(foo8b, flipTable<uint8_t>);
  bits::flipArray(foo8c, flipMask<uint8_t>);

  uint32_t rrange = rand() % ARRAY_SIZE;
  EXPECT_EQ(foo8b[rrange], foo8[rrange]);
  EXPECT_EQ(foo8b[rrange], foo8c[rrange]);
  EXPECT_EQ(fooZ8[rrange], foo8[rrange]);
  EXPECT_EQ(fooZ8[rrange], foo8b[rrange]);
  EXPECT_EQ(fooZ8[rrange], foo8c[rrange]);
}

TEST_F(TestSuite, testAVXArray2) {
  uint8_t foo8b[ARRAY_SIZE];
  std::copy(std::begin(foo8), std::end(foo8), std::begin(foo8b));

  bits::flipAVXArray(fooZ8);
  bits::flipNaiveArray(foo8);
  bits::flipTableArray(foo8b);

  uint32_t rrange = rand() % ARRAY_SIZE;
  EXPECT_EQ(foo8b[rrange], foo8[rrange]);
  EXPECT_EQ(fooZ8[rrange], foo8[rrange]);
}
#endif

TEST_F(TestSuite, testLambda64) {
  hex64 = flipNaiveLambda(hex64);
  EXPECT_EQ(rhex64, hex64);
}

TEST_F(TestSuite, testSwap) {
  v4si x = {0x1234, 0x5678, 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE, 0xFFFF};
  v4si a = {0xFFFF, 0xEEEE, 0xDDDD, 0xCCCC, 0xBBBB, 0xAAAA, 0x5678, 0x1234};
  v4si y = builtinSwap(x);
  // EXPECT_EQ(a, y);
}

TEST_F(TestSuite, test_bitflipllloop) {
  _bitflipllloop(foo64, ARRAY_SIZE / 8);
  EXPECT_EQ(rhex64, foo64[0]);
}

TEST_F(TestSuite, testTableLookup16) {
  EXPECT_EQ(rhex16, flipTable<uint16_t>(hex16));
}

TEST_F(TestSuite, testTableLookup32) {
  EXPECT_EQ(rhex32, flipTable<uint32_t>(hex32));
}

TEST_F(TestSuite, testTableLookup8) {
  EXPECT_EQ(rhex8, flipTable<uint8_t>(hex8));
}

TEST_F(TestSuite, testFlipMask) {
  EXPECT_EQ(rhex8, flipMask(hex8));
}

/*----------------------------------------------------------------- Bit Population Counters ------------------------------------------------------------*/

/*-------------------------------------------------------------------   Begin BENCHMARKS   -------------------------------------------------------------*/
#ifndef NOBENCHMARK

void BM_popcntWegner(benchmark::State& state) {
  while (state.KeepRunning())
	for (i = 0; i < ARRAY_SIZE / 4; i++)
	  popcntWegner<uint32_t>(foo32[i]);
}

void BM_popcntIntrinsic(benchmark::State& state) {
  while (state.KeepRunning())
	for (i = 0; i < ARRAY_SIZE / 4; i++)
	  popcntIntrinsic<uint32_t>(foo32[i]);
}

BENCHMARK(BM_popcntWegner);
BENCHMARK(BM_popcntIntrinsic);
#endif

/*----------------------------------------------------------------------    Begin TESTS    -------------------------------------------------------------*/

TEST_F(TestSuite, testpopcntWegner) {
  uint32_t bits = 0b01101101010010100110110101001010;
  EXPECT_EQ(16, popcntWegner<uint32_t>(bits));
  bits = 0b0110110101001010;
  EXPECT_EQ(8, popcntWegner<uint16_t>(bits));
}

TEST_F(TestSuite, testpopcnt16) {
  EXPECT_EQ(popcntWegner<uint16_t>(hex16), popcnt16(hex16));
}

TEST_F(TestSuite, testpopcnt32) {
  EXPECT_EQ(popcntWegner<uint32_t>(hex32), popcnt32(hex32));
}

TEST_F(TestSuite, testpopcntIntrinsics) {
  EXPECT_EQ(popcntWegner<uint32_t>(hex32), popcntIntrinsic<uint32_t>(hex32));
  EXPECT_EQ(popcntWegner<uint64_t>(hex64), popcntIntrinsic<uint64_t>(hex64));
}

TEST_F(TestSuite, testpopcntIntrinsic) {
  uint16_t a, b;
  for (i = 0; i < ARRAY_SIZE / 4; i++) {
	a = popcntIntrinsic<uint32_t>(foo32[i]);
	b = popcntWegner<uint32_t>(foo32[i]);

	// std::cout << "(" << std::setfill('0') << std::setw(5) << i << ") COUNT: " << std::setfill('0') << std::setw(2) << a << " - " << std::bitset<32>(b) << std::endl;
	EXPECT_EQ(a, b);
  }
  SUCCEED();
}


