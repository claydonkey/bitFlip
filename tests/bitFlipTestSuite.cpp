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
uint8_t hex8;
uint16_t t, hex16, rev1, rev2;
uint64_t hex64;
uint32_t hex32;
uint16_t cntbits;
uint8_t foo1[ARRAY_SIZE];
uint8_t foo2[ARRAY_SIZE];
uint8_t foo3[ARRAY_SIZE];
uint64_t foo64[ARRAY_SIZE / 8];
uint32_t foo32[ARRAY_SIZE / 4];
uint16_t foo16[ARRAY_SIZE / 2];

uint16_t foo16T[0xFFFF + 1];
__attribute__ ((aligned(32))) uint8_t fooZ[ARRAY_SIZE + 32] = {};

__attribute__ ((aligned(32))) uint16_t foo16Z[ARRAY_SIZE + 32] = {};
// Constants (first we got the mask, then the high order nibble look up table and last we got the low order nibble lookup table)

class TestSuite : public testing::Test {
public:

    void SetUp() {
        for (uint32_t i = 0; i < ARRAY_SIZE; i++) {
            foo16Z[i] = fooZ[i] = foo3[i] = foo2[i] = foo1[i] = rand();

            if (!(i % 8) & !i == 0) {
                foo64[(i / 8) - 1] = ((uint64_t) foo1[i - 8]) + ((uint64_t) foo1[i - 7] << 8) + \
                        ((uint64_t) foo1[i - 6] << 16) + ((uint64_t) foo1[i - 5] << 24) + \
   ((uint64_t) foo1[i - 4] << 32) +((uint64_t) foo1[i - 3] << 40) +((uint64_t) foo1[i - 2] << 48) + \
                        ((uint64_t) foo1[i - 1] << 56);
            }

            if (!(i % 4) & !i == 0) {
                foo32[(i / 4) - 1] = ((uint64_t) foo1[i - 4]) + ((uint64_t) foo1[i - 3] << 8) + ((uint64_t) foo1[i - 2] << 16) + ((uint64_t) foo1[i - 1] << 24);
            }

            if (!(i % 2) & !i == 0) {
                foo16[(i / 2) - 1] = ((uint64_t) foo1[i - 2]) + ((uint64_t) foo1[i - 1] << 8);
            }
        }

        hex8 = 0b01010101;
        hex16 = 0b0000110001111001;
        hex32 = 0b11010101111111100000110011111000;
        hex64 = 0b0101010111111110000011001111100000111110101000011111000000011101;
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

void BM_bitFlip_assemb(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipZ(fooZ);
}

void BM_bitFlipNaiveArrayll(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipNaiveArrayll(foo64, ARRAY_SIZE / 8);

}

void BM_bitFlipNaiveLambda(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipNaiveLambda(foo64, ARRAY_SIZE / 8);
}

void BM_bitFliplloop(benchmark::State& state) {
    while (state.KeepRunning())
        bitflipllloop(foo64, ARRAY_SIZE / 8);
}

void BM_bitFlip_mask(benchmark::State& state) {
    while (state.KeepRunning())
        //     for (uint32_t i = 0; i < ARRAY_SIZE; i++) foo2[i] = ((foo2[i] * 0x202020202ULL & 0x010884422010ULL) % 1023);
        bitFlipMaskArray<uint8_t>(foo2);
}

void BM_bitflip_naive(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipNaiveArray<uint8_t>(foo3);
}

void BM_bitFlip_table_16(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipTableArray<uint16_t>(foo16);
}

void BM_bitflip_table_32(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipTableArray<uint32_t>(foo32);
}
#ifndef __MINGW64__
BENCHMARK(BM_bitFlip_assemb);
#endif
BENCHMARK(BM_bitFlip_table_16);
BENCHMARK(BM_bitflip_table_32);
BENCHMARK(BM_bitflip_naive);
BENCHMARK(BM_bitFlip_mask);
#ifndef __MINGW64__
BENCHMARK(BM_bitFliplloop);
#endif
BENCHMARK(BM_bitFlipNaiveArrayll);
BENCHMARK(BM_bitFlipNaiveLambda);

TEST_F(TestSuite, textPlainC) {
#ifndef __MINGW64__
    uint8_t b = bitFlipZb(((uint8_t) 0b10001100));
    EXPECT_EQ(0b00110001, b);
#endif
}

TEST_F(TestSuite, test_bitflip_naive) {
    uint8_t b = bitFlipNaive((uint8_t) 0b01010101);
    EXPECT_EQ(0b10101010, b);
}

TEST_F(TestSuite, testLambda64) {
    uint64_t r = 0;
    uint8_t c = 0;
    uint8_t s = sizeof (uint64_t)*8;

    hex64 = [&] (uint64_t i) -> uint64_t {
        r = 0;
        for (c = 0; c < s; c++) r |= ((i >> c) & 1) << (s - c - 1);
        return r;
    }(hex64);

    EXPECT_EQ(13262966195775111082ul, hex64);
}

TEST_F(TestSuite, testTableLookup16) {
    hex16 = bitFlipTable<uint16_t>(hex16);
    EXPECT_EQ(0b1001111000110000, hex16);
}

TEST_F(TestSuite, testTableLookup32) {
    hex32 = bitFlipTable<uint32_t>(hex32);
    EXPECT_EQ(0b00011111001100000111111110101011, hex32);
}

TEST_F(TestSuite, testTableLookup8) {
    hex8 = bitFlipTable<uint8_t>(hex8);
    EXPECT_EQ(0b10101010, hex8);
}

TEST_F(TestSuite, letsTestThoseBits) {
#ifndef __MINGW64__
    bitFlipZ(fooZ);
#endif
    bitFlipMaskArray<uint8_t>(foo2);
    bitFlipNaiveArray<uint8_t>(foo3);
#ifndef __MINGW64__
    EXPECT_EQ(fooZ[ARRAY_SIZE - 2], foo2[ARRAY_SIZE - 2]);
#endif
    EXPECT_EQ(foo3[ARRAY_SIZE - 2], foo2[ARRAY_SIZE - 2]);
    EXPECT_EQ(foo3[ARRAY_SIZE - 2], foo2[ARRAY_SIZE - 2]);

    SUCCEED();
}


//TEST_F(TestSuite, testshortFlip) {
//
//    foo16Z[0] = 0b1100001110100101;
//    cout << "Before foo16Z: " << bitset<16>(foo16Z[0]) << endl;
//    bitFlipZ(foo16Z);
//    cout << "After foo16Z: " << bitset<16>(foo16Z[0]) << endl;
//    EXPECT_EQ(0b101001011100001, foo16Z[0]);
//
//}
