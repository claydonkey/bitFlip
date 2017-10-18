/*
 * File:   bitFliptestsuite
 * Author: Anthony Campbell
 *
 * Created on 06-Oct-2017, 16:08:35
 */

#include <gtest/gtest.h>
#include "bitFlip.h"
#include <stdint.h>
#include <bitset>
#include <math.h>
#include <omp.h>
#include<iostream>
#include <cmath>
#include <benchmark/benchmark.h>
#include "bitFlipTest.h"
#include "BitReverseTable16.h"

using namespace std;

#define ARRAY_SIZE 1000
uint16_t i;
uint16_t rev1, rev2;
uint8_t hex8, rhex8;
uint16_t hex16, rhex16;
uint32_t hex32, rhex32;
uint64_t hex64, rhex64;
uint16_t cntbits;

uint8_t foo2[ARRAY_SIZE ];
uint8_t foo3[ARRAY_SIZE ];

uint8_t foo8[ARRAY_SIZE ];
uint16_t foo16[ARRAY_SIZE / 2];
uint32_t foo32[ARRAY_SIZE / 4];
uint64_t foo64[ARRAY_SIZE / 8];

uint16_t foo16T[0xFFFF + 1];
__attribute__ ((aligned(32))) uint8_t fooZ[ARRAY_SIZE + 32] = {};


// Constants (first we got the mask, then the high order nibble look up table and last we got the low order nibble lookup table)

class TestSuite : public testing::Test {
public:

    void SetUp() {
        for (uint32_t i = 0; i < ARRAY_SIZE; i++) {
            foo8[i] = fooZ[i] = foo3[i] = foo2[i] = rand();

            if (!(i % 8) & !i == 0) {
                foo64[(i / 8)] = ((uint64_t) foo8[i - 8]) + ((uint64_t) foo8[i - 7] << 8) + \
                ((uint64_t) foo8[i - 6] << 16) + ((uint64_t) foo8[i - 5] << 24) + ((uint64_t) foo8[i - 4] << 32) + \
                ((uint64_t) foo8[i - 3] << 40) +((uint64_t) foo8[i - 2] << 48) + ((uint64_t) foo8[i - 1] << 56);
            }

            if (!(i % 4) & !i == 0) {
                foo32[(i / 4)] = ((uint64_t) foo8[i - 4]) + ((uint64_t) foo8[i - 3] << 8) + ((uint64_t) foo8[i - 2] << 16) + ((uint64_t) foo8[i - 1] << 24);
            }

            if (!(i % 2) & !i == 0) {
                foo16[(i / 2)] = ((uint64_t) foo8[i - 2]) + ((uint64_t) foo8[i - 1] << 8);
            }
        }

        fooZ[0] = foo8[0] = hex8 = rand();
        rhex8 = bitFlipNaive<uint8_t>(hex8);
        foo16[0] = hex16 = rand();
        rhex16 = bitFlipNaive<uint16_t>(hex16);
        foo32[0] = hex32 = ((uint32_t) rand() << 16) | rand();
        rhex32 = bitFlipNaive<uint32_t>(hex32);
        foo64[0] = hex64 = ((uint64_t) rand() << 32) | rand();
        rhex64 = bitFlipNaive<uint64_t>(hex64);
#ifdef EXTERNALTABLE
        //Generate 8bit table
        for (uint16_t i = 0; i < (0xFF + 1); i++) {
            BitReverseTable8[i] = bitFlipMask<uint8_t>((uint8_t&) i);
#ifdef PRINTTABLE8
            cout << "0x" << std::hex << BitReverseTableGen16 << ", ";
            if (!(i % 16)) cout << "\\" << endl;
#endif
        }
        //Generate 1bit table
        for (uint64_t i = 0x00; i < (0xFFFF + 1); i++) {
            uint8_t i2 = static_cast<uint8_t> (i >> 8);
            uint16_t BRH = static_cast<uint16_t> ((bitFlipMask<uint8_t>(static_cast<uint8_t&> (i2)) & 0xff));
            uint16_t BRL = static_cast<uint16_t> (bitFlipMask<uint8_t>((uint8_t&) i)) << 8;
            uint16_t BR = BRL + BRH;
            BitReverseTable16[i] = BR;

            //   cout << bitset<16>(BitReverseTable16[i]) << endl;
#ifdef PRINTTABLE16
            cout << "0x" << std::hex << BitReverseTable16[i] << ", ";
            if (!(i % 16)) cout << "\\" << endl;
#endif

        }
#endif
    }
};
/*---------------------------------------------------------------------------------------      bitFlip Counters      ---------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------- Begin BENCHMARKS ---------------------------------------------------------------------------------------*/

void BM_bitFlip_AVX(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipAVXArray(foo8);
}

void BM_bitFlip_NaiveArrayll(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipNaiveArrayll(foo64, ARRAY_SIZE / 8);
}

void BM_bitFlip_NaiveLambda(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipNaiveLambda(foo64, ARRAY_SIZE / 8);
}

void BM_bitFlip_lloop(benchmark::State& state) {
    while (state.KeepRunning())
        _bitflipllloop(foo64, ARRAY_SIZE / 8);
}

void BM_bitFlip_Mask(benchmark::State& state) {
    while (state.KeepRunning())
        //     for (uint32_t i = 0; i < ARRAY_SIZE; i++) foo2[i] = ((foo2[i] * 0x202020202ULL & 0x010884422010ULL) % 1023);
        bitFlipMaskArray<uint8_t>(foo2);
}

void BM_bitFlip_Naive(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipNaiveArray<uint8_t>(foo3);
}

void BM_bitFlip_Table16(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipTableArray<uint16_t>(foo16);
}

void BM_bitFlip_Table32(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipTableArray<uint32_t>(foo32);
}


#ifndef NOASSMBLR
BENCHMARK(BM_bitFlip_AVX);
#endif
BENCHMARK(BM_bitFlip_Table16);
BENCHMARK(BM_bitFlip_Table32);
BENCHMARK(BM_bitFlip_Naive);
BENCHMARK(BM_bitFlip_Mask);
#ifndef NOASSMBLR
BENCHMARK(BM_bitFlip_lloop);
#endif
BENCHMARK(BM_bitFlip_NaiveArrayll);
BENCHMARK(BM_bitFlip_NaiveLambda);

/*--------------------------------------------------------------------------------------- Begin TESTS ---------------------------------------------------------------------------------------*/

TEST_F(TestSuite, testbitFlipNaive) {
    EXPECT_EQ(rhex8, bitFlipNaive(hex8));
}

#ifndef NOASSMBLR

TEST_F(TestSuite, testbitFlipAVX) {
    EXPECT_EQ(rhex8, bitFlipAVX(hex8));
}

TEST_F(TestSuite, testAVXArray) {
    bitFlipAVXArray(foo8);
    EXPECT_EQ(rhex8, foo8[0]);
}
#endif

TEST_F(TestSuite, testLambda64) {
    uint64_t r = 0;
    uint8_t c = 0;
    uint8_t s = sizeof (uint64_t)*8;

    hex64 = [&] (uint64_t i) -> uint64_t {
        r = 0;
        for (c = 0; c < s; c++) r |= ((i >> c) & 1) << (s - c - 1);
        return r;
    }(hex64);

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
    EXPECT_EQ(rhex16, bitFlipTable<uint16_t>(hex16));
}

TEST_F(TestSuite, testTableLookup32) {
    EXPECT_EQ(rhex32, bitFlipTable<uint32_t>(hex32));
}

TEST_F(TestSuite, testTableLookup8) {
    EXPECT_EQ(rhex8, bitFlipTable<uint8_t>(hex8));
}

TEST_F(TestSuite, testFlipMask) {
    EXPECT_EQ(rhex8, bitFlipMask(hex8));
}

/*--------------------------------------------------------------------------------------- Bit Population Counters ---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------   Begin BENCHMARKS   ---------------------------------------------------------------------------------------*/

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

/*---------------------------------------------------------------------------------------            Begin TESTS            ---------------------------------------------------------------------------------------*/

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

        std::cout << "(" << std::setfill('0') << std::setw(5) << i << ") COUNT: " << std::setfill('0') << std::setw(2) << a << " - " << std::bitset<32>(foo32[i]) << std::endl;
        EXPECT_EQ(a, b);
    }
}

TEST_F(TestSuite, letsTestThoseBits) {
#ifndef NOASSMBLR
    bitFlipAVXArray(fooZ);
#endif
    bitFlipMaskArray<uint8_t>(foo2);
    bitFlipNaiveArray<uint8_t>(foo3);
#ifndef NOASSMBLR
    EXPECT_EQ(fooZ[ARRAY_SIZE - 2], foo2[ARRAY_SIZE - 2]);
#endif
    EXPECT_EQ(foo3[ARRAY_SIZE - 2], foo2[ARRAY_SIZE - 2]);
    EXPECT_EQ(foo3[ARRAY_SIZE - 2], foo2[ARRAY_SIZE - 2]);

    SUCCEED();
}
