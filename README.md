# bitFlip

bitFlip is a C++ library that has a singular purpose:

The fast reversal of the order of bits in an arbitrary std::integer.

It was created to evaluate and benchmark different approaches to this simple task

+ benchmarked with google benchmark
+ unit tested with gtest

In order to use it you must have:

  - a machine that has a processor that supports intel avx extensions
  - a version of nasm installed
  - a gcc compiler that supports Intel intrinsics

Advanced Vector Extensions as defined by Wikipedia on the [Wikipedia Article][wiki]

> Advanced Vector Extensions (AVX) are extensions to the x86 instruction set architecture for microprocessors from Intel and AMD proposed by Intel in March 2008 and first supported by Intel with the Sandy Bridge[1] processor shipping in Q1 2011 and later on by AMD with the Bulldozer[2] processor shipping in Q3 2011. AVX provides new features, new instructions and a new coding scheme.


### Tech

bitFlip uses a number of different approaches to flip those bits

* [Assembly] - win32 / elf. Compile with nasm - this routine is the fastest - just
* [Intel Implicit AVX] - Using the handy gcc routines
* [Naive approach] - A fun way to visualize a solution (also nice and terse)
* [Table approach] - Let's see how the compiler optimizes these
* [Lambda Syntax] - Just gotta try it out
* [Scala equivalent] - Look at the sveltness of the code - and the how fairs the JVM?

### NB population counter
As a way to see how fast avx instruction can be. I also include the popcnt instruction which requires no lashing together of assmebly.
This is benchmarked at

### Installation

bitFlip has both a cmake installer and makefiles.
The cmake installation will detect if you require google benchmark and gtest and will download and installed if required

### cmake
```
$ mkdir cmake-build-release
$ cd cmake-build-release
$ cmake ..
cmake .. --help-usagecmake -L | awk '{if(f)print} /-- Cache values/{f=1}'
Specify --help for usage, or press the help button on the CMake GUI.
CMAKE_BUILD_TYPE:STRING=Release
CMAKE_GNUtoMS:BOOL=OFF
CMAKE_INSTALL_PREFIX:PATH=C:/Program Files (x86)/bitFlip
NASM_EXE:FILEPATH=C:/msys2/mingw64/bin/nasm.exe
TESTAPP_ASMFORMAT:STRING=win64
TESTAPP_USE_ASM:BOOL=true
TESTAPP_USE_BENCHMARK:BOOL=true
TESTAPP_USE_TABLEHEADERS:BOOL=true
benchmark_ROOT_DIR:PATH=
shlwapi_ROOT_DIR:PATH=

```

example:
```
mkdir cmake-build-release
cmake .. -G 'MSYS Makefiles'
make
```

### make
```
$ make help
This makefile supports the following configurations:
    MinGW64_Release MinGW64_Debug Arch_Release Arch_Debug

and the following targets:
    build  (default target)
    clean
    clobber
    all
    help

Makefile Usage:
    make [CONF=<CONFIGURATION>] [SUB=no] build
    make [CONF=<CONFIGURATION>] [SUB=no] clean
    make [SUB=no] clobber
    make [SUB=no] all
    make help
```

example (for MinGW):
```
$ make CONF=MinGW64_Release
```

##### Commandline executable

```
tests/./flipbits --help
```
example:

```
tests/./flipBits.exe  -o test.txt -f I 0 100
```

saves as flipped decimal representations to test.txt

##### Benchmark/Tests

```
tests/./TestApp

```

##### Just for fun - Scala
Prints out bit flipped integers (as large as BigInt) from a range [FROM] [TO]

to run:
open sbt and execute:
sbt:bitflip> run [FROM] [TO]

##### Scala example:

```
$ sbt
Listening for transport dt_socket at address: 5005
[info] ...
sbt:bitflip> run 1000000000010 1000000000020

```

output:

```
[debug] Waiting for thread run-main-0 to terminate.
...
1000000000010-->343742425879
1000000000011-->893498239767
1000000000012-->206303472407
1000000000013-->756059286295
1000000000014-->481181379351
1000000000015-->1030937193239
1000000000016-->34504780567
1000000000017-->584260594455
1000000000018-->309382687511
1000000000019-->859138501399
1000000000020-->171943734039

[debug]         Thread run-main-0 exited.

```

##### An apple for apples command using the compiled command:

```
$ tests/./flipBits -f I 1000000000010 1000000000020

```

### Benchmark Results

Google Benchmark to iterate over 10240 byte reversals


```

Run on (8 X 3292 MHz CPU s)
11/10/17 10:12:26
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
BM_Flip_AVX                           215 ns        214 ns    2986667 <- 8bit
BM_Flip_AVX16                        2548 ns       2511 ns     280000
BM_Flip_IntrAVXArr_VecClass           772 ns        767 ns     896000
BM_Flip_IntrAVXVec_VecClass           778 ns        785 ns     896000
BM_Flip_IntrAVXClass_ptr              412 ns        414 ns    1659259 <-64bit The Winner
BM_Flip_IntrAVXClassNullBuffer     678206 ns     662667 ns        896
BM_Flip_IntrAVX64                     356 ns        353 ns    2036364
BM_Flip_IntrAVX64i256                 746 ns        753 ns    1120000
BM_Flip_Table16                      3029 ns       2999 ns     224000
BM_Flip_Table32                      3812 ns       3850 ns     186667
BM_Flip_Naive                        9830 ns       9766 ns      64000
BM_Flip_Naive64                     11952 ns      11963 ns      64000
BM_Flip_Mask                        16748 ns      16497 ns      40727
BM_Flip_lloop                       25314 ns      24902 ns      26353
BM_Flip_NaiveArrayll                26762 ns      26228 ns      28000
BM_Flip_NaiveLambda                 25067 ns      25181 ns      23579
BM_popcntWegner                     30923 ns      30692 ns      22400
BM_popcnt_wegner_lambda            128452 ns     128348 ns       5600
BM_popcnt_SWAR64                     4036 ns       4018 ns     186667
BM_popcnt16                         18439 ns      18415 ns      40727
BM_popcnt32                          8273 ns       8196 ns      89600
BM_popcnt64                          5287 ns       5312 ns     100000
BM_popcntIntrinsic                      2 ns          2 ns  320000000

```

ScalaTest Execution times to iterate 10240

```
[info] - bitFlip inline benchmark with warming
[info] Run completed in 6 seconds, 165 milliseconds.
[info] Total number of tests run: 1
[info] Suites: completed 1, aborted 0
[info] Tests: succeeded 1, failed 0, canceled 0, ignored 0, pending 0
[info] All tests passed.

```

### Development

Please feel free...

License
----

MIT


[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)


   [wiki]: <https://en.wikipedia.org/wiki/Advanced_Vector_Extensions>


