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
 * File:   bitFliptest.cpp
 * Author: Anthony Campbell
 *
 * Created on 06 October 2017, 16:07
 */

#include <stdlib.h>
#include <iostream>
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "bitFlipTest.h"
#include "bitFlip.h"


using namespace std;

int main(int argc, char** argv) {
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    testing::InitGoogleTest(&argc, argv);

   ::benchmark::RunSpecifiedBenchmarks();
    return   RUN_ALL_TESTS();
}

