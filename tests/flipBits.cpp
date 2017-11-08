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
#include <fstream>

#include "bitFlip.h"
#include "bitFlipAVX.h"
#include "flipBits.h"
#include "cxxopts.hpp"
#include <tuple>
using namespace std;

int main(int argc, char** argv) {


  std::vector < std::uint64_t>vRange{};
  uint8_t mBytes = 0;
  uint8_t mH = 0;
  char mF = 'B';
  bool mInput = false;
  char * pEnd;
  const char *nptr;
  uint16_t base = 10;
  std::string filename;
  std::pair<uint64_t, uint64_t> range(0, 0);
  cxxopts::Options options("flipBits", "produce a range of bit-reversed Integers");
  options.positional_help("[optional args]");

  options.add_options()
		  ("help", "Print help")
		  ("h,header", "Display Header Flags (1:Debug 2:Header)", cxxopts::value<uint8_t>(mH))
		  ("f,format", "Display Flags (B:Binary D:Decimal I:Trailing Zero removal (caps returns single column) ", cxxopts::value<char>(mF))
		  ("b,base", "Integer Length", cxxopts::value<uint8_t>(mBytes))
		  ("o,output", "Output file", cxxopts::value<std::string>(filename))// ->default_value("bitFlip.txt")->implicit_value("b.def"), "BIN")
		  ("positional", "The range to display ([From] To)", cxxopts::value<std::vector < std::uint64_t >> (vRange));


  options.parse_positional({"lower", "upper", "positional"});
  options.parse(argc, argv);

  if (options.count("help") || !vRange.size()) {
	std::cout << options.help({"", "Group"}) << std::endl;
	exit(0);
  }


  std::get<0>(range) = vRange[0];
  std::get<1>(range) = (vRange.size() > 1) ? vRange[1] : 10;


  if ((std::get<1>(range) < std::get<0>(range))) {
	printf("number range: %lu is less than %lu (range reversed) \n", std::get<1>(range), std::get<0>(range));
	swap(std::get<1>(range), std::get<0>(range));
  }

  if (!mBytes) {
	uint64_t big = vRange[0] > vRange[1] ? vRange[0] : vRange[1];
	mBytes = (big >= 0xFFFFFFFF) ? 8 : (big >= 0xFFFF) ? 4 : (big >= 0xFF) ? 2 : 1;
  }

  //@note std::unique_ptr<ostream > ptr_ostream; ==  equivalent to ostream * ptr_ostream;
  //std::unique_ptr<ostream > ptr_ostream;
  ostream * ptr_ostream;
  ofstream fstream;

  if (!(filename.empty())) {
	//@note ptr_ostream = std::unique_ptr<ostream >(&fstream); == equivalent to ptr_stream = &fstream;
	ptr_ostream = &fstream;
	//ptr_ostream = std::unique_ptr<ostream >(&fstream);
	fstream.open(filename);

  } else
	ptr_ostream = &std::cout;
  //ptr_ostream = std::unique_ptr<ostream >(&std::cout);

  switch (mBytes) {
	case 1:
	{
	  bits::AVX<uint8_t> flip1(range, mH, mF);
	  *ptr_ostream << flip1;
	  break;
	}
	case 2:
	{
	  bits::AVX<uint16_t> flip2(range, mH, mF);
	  *ptr_ostream << flip2;
	  break;
	}
	case 4:
	{
	  bits::AVX<uint32_t> flip3(range, mH, mF);
	  *ptr_ostream << flip3;
	  break;
	}
	case 8:
	{
	  bits::AVX<uint64_t> flip4(range, mH, mF);
	  *ptr_ostream << flip4;
	  break;
	}
  }
  if (!(filename.empty()))
	fstream.close();
  return 0;

}
