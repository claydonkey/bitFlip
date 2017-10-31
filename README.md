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
* [Naive approaches] - A fun way to visualize a solution (also nice and terse)
* [Lambda Syntax] - Just gotta try it out
* [Scala equivalent] - Look at the sveltness of the code - and the tardiness.

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

----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
BM_Flip_IntrAVX64                     350 ns        345 ns    1947826
BM_Flip_IntrAVX64i256                 794 ns        802 ns     896000
BM_Flip_IntrAVXArr_VecClass          1504 ns       1507 ns     497778
BM_Flip_IntrAVXVec_VecClass          3924 ns       3924 ns     179200
BM_Flip_IntrAVXClassShared_ptr       1106 ns       1099 ns     640000
BM_Flip_IntrAVXClassUnique_ptr      todo  ns        --- ns     -----
BM_Flip_IntrAVXClassNullBuffer     764653 ns     767299 ns        896
BM_Flip_Table16                      2970 ns       2982 ns     235789
BM_Flip_Table32                      3940 ns       3924 ns     179200
BM_Flip_Naive                       10103 ns      10010 ns      64000
BM_Flip_Naive64                      9647 ns       9626 ns      74667
BM_Flip_Mask                        15675 ns      15695 ns      44800
BM_Flip_NaiveArrayll                25985 ns      26088 ns      26353
BM_Flip_NaiveLambda                 25991 ns      26088 ns      26353
BM_popcntWegner                     33511 ns      32993 ns      20364
BM_popcntIntrinsic                      1 ns          1 ns  560000000

```

### Development

Please feel free...

License
----

MIT


[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)


   [wiki]: <https://en.wikipedia.org/wiki/Advanced_Vector_Extensions>


