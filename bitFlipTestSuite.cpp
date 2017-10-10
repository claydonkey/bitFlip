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

using namespace std;

#define ARRAY_SIZE 1000

uint8_t hex8;
uint16_t t, hex16, rev1, rev2;
uint64_t hex64;
uint16_t cntbits;
uint8_t foo1[ARRAY_SIZE];
uint8_t foo2[ARRAY_SIZE];
uint8_t foo3[ARRAY_SIZE];
__attribute__ ((aligned(32))) uint8_t fooZ[ARRAY_SIZE + 32] = {};

__attribute__ ((aligned(32))) uint16_t foo16Z[ARRAY_SIZE + 32] = {};
// Constants (first we got the mask, then the high order nibble look up table and last we got the low order nibble lookup table)

class TestSuite : public testing::Test {
public:

};

void SetUp() {
    for (uint32_t i = 0; i < ARRAY_SIZE; i++)
        foo16Z[i] = fooZ[i] = foo3[i] = foo2[i] = foo1[i] = rand();

    hex8 = 0b01010101;
    hex16 = 0b0000110001111001;
    hex64 = 0b0101010111111110000011001111100000111110101000011111000000011101;
    // cout << "hex16: " << hex16 << endl;
}

void TearDown() {
}

void BM_bitFlip_Assym(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipZ(fooZ);
}

void BM_bitFlipMask(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipMaskArray(foo2, ARRAY_SIZE);
}

void BM_naive(benchmark::State& state) {
    while (state.KeepRunning())
        bitFlipNaiveArray(foo3, ARRAY_SIZE);
}


BENCHMARK(BM_bitFlipMask);
BENCHMARK(BM_bitFlip_Assym);
BENCHMARK(BM_naive);

TEST_F(TestSuite, testExample) {

    cout << "foo16Z: " << bitset<16>(foo16Z[ARRAY_SIZE - 2]) << endl;
    bitFlipZ(foo16Z);

    cout << "foo16Z: " << bitset<16>(foo16Z[ARRAY_SIZE - 2]) << endl;

    cout << "fooZ: " << bitset<8>(fooZ[ARRAY_SIZE - 2 ]) << endl;
    cout << "foo2: " << bitset<8>(foo2[ARRAY_SIZE - 2]) << endl;
    cout << "foo3: " << bitset<8>(foo3[ARRAY_SIZE - 2]) << endl;

    bitFlipZ(fooZ);
    bitFlipMaskArray(foo2, ARRAY_SIZE);
    bitFlipNaiveArray(foo3, ARRAY_SIZE);
    uint8_t b = bitFlipNaive((uint8_t) 0b01010101);
    cout << "b: " << bitset<8>(b) << endl;

    cout << "barZ: " << bitset<8>(fooZ[ARRAY_SIZE - 2]) << endl;
    cout << "bar2: " << bitset<8>(foo2[ARRAY_SIZE - 2]) << endl;
    cout << "bar3: " << bitset<8>(foo3[ARRAY_SIZE - 2]) << endl;

    ASSERT_TRUE(fooZ[ARRAY_SIZE - 2] == foo2[ARRAY_SIZE - 2]);
    ASSERT_TRUE(foo3[ARRAY_SIZE - 2] == foo2[ARRAY_SIZE - 2]);
    ASSERT_TRUE(foo3[ARRAY_SIZE - 2] == foo2[ARRAY_SIZE - 2]);
    ASSERT_TRUE(0b10101010 == b);

    cntbits = bitCount <uint16_t>(0b0000110001111001);
    cout << "bar: " << bitset<8>(foo1[2]) << endl;
    cout << "bits: " << cntbits << endl;
    cout << "hex8: " << bitset<8>(bitFlipNaiveb(hex8)) << endl;
    cout << "hex16 RValue: " << bitset<16> (bitFlipNaive(((uint16_t)0b0000110001111001))) << endl;
    cout << "hex16 LValue Template: " << bitset<16>(bitFlipNaive<uint16_t> (hex16)) << endl;
    cout << "hex16 RValue Template: " << bitset<16>(bitFlipNaive<uint16_t> (0b0000110001111001)) << endl;


    SUCCEED();
}

