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


using namespace std;

uint8_t hex8;
uint16_t t, hex16, rev1, rev2;
uint64_t hex64;
uint16_t cntbits;
// Constants (first we got the mask, then the high order nibble look up table and last we got the low order nibble lookup table)


class TestSuite : public testing::Test {
public:

};

void SetUp() {
    hex8 = 0b01010101;
    hex16 = 0b0000110001111001;
    hex64 = 0b0101010111111110000011001111100000111110101000011111000000011101;
    cout << "hex16: " << hex16 << endl;
}

void TearDown() {

}

TEST_F(TestSuite, testExample) {


    for (uint32_t i = 0; i < NUM_DATA_BYTES; i++) {
	data[i] = rand();
    }
    bitflipbyte(data, (uint8_t) ceil(NUM_DATA_BYTES / 32.0), k1);


    cntbits = bits <uint16_t>(0b0000110001111001);

    cout << "bits: " << cntbits << endl;
    cout << "hex8: " << std::bitset<8>(bitFlipb(hex8)) << endl;
    cout << "hex16r: " << std::bitset<16>(bitFlipl(0b0000110001111001)) << endl;
    cout << "hex16l: " << std::bitset<16>(bitFlip<uint16_t> (hex16)) << endl;
    cout << "hex16l: " << std::bitset<16>(bitFlip<uint16_t> (0b0000110001111001)) << endl;

    SUCCEED();
}

