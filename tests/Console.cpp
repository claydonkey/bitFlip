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

#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <cerrno>
#include <cstring>
#include <bitset>
#include <math.h>
#include <omp.h>
#include  <iomanip>
#include <vector>

#include "bitFlip.h"
#include "bitFlipAVX.h"
#include "Console.h"
#include "cxxopts.hpp"

using namespace std;

cxxopts::Options options("BitFlip", "Produce a range of Bit Reversed Integers");

int main(int argc, char** argv) {

    std::vector < std::uint64_t>vRange{0, 0};
    uint8_t mBytes = 1;
    uint8_t mDisplay = 1;
    char * pEnd;
    const char *nptr;
    uint16_t base = 10;
    uint64_t range[2] = {0, 0};
    /*
        argc = 3;
        const char * argvN[3] = {"1", "10", "128"};
        const char ** pargvN = argvN;
        argv = pargvN;
     */

    options.add_options()
            ("d,Display", "Display Type", cxxopts::value<uint8_t>(mDisplay))
            ("b,Base", "Integer Length", cxxopts::value<uint8_t>(mBytes))
            ("help", "Print help")
            ("positional", "The range to display ([From] To)", cxxopts::value<std::vector < std::uint64_t >> (vRange));

    options.parse(argc, argv);

    if (options.count("help")) {
        std::cout << options.help({"", "Group"}) << std::endl;
        exit(0);
    }

    options.parse_positional({"lower", "upper", "positional"});


    if (options.count("lower")) {
        std::cout << "lower = " << options["lower"].as<std::string>() << std::endl;
    }

    if (options.count("upper")) {
        std::cout << "upper = " << options["upper"].as<std::string>() << std::endl;
    }

    range[0] = vRange[0];
    range[1] = vRange[1];



    if ((range[1] < range[0])) {
        printf("number range: %lu is less than %lu (range reversed) \n", range[1], range[0]);
        swap(range[1], range[0]);
    }

    printFlipByte<uint8_t>(mDisplay, range);


    return 0;

}
