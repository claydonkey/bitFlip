 # bitFlip

bitFlip is a library that has the sole purpose of reversing the order of bits in an integer.
It comes with benchmarking and unit tests
In order to use it:

  - your machine must be i386-64 or AMD
  - +support intel avx extensions
  - a version of nasm installed
  - a compiler that supports Intel intrinsics

Advanced Vector Extensions as defined by Wikipedia on the [Wikipedia Article][wiki]

> Advanced Vector Extensions (AVX) are extensions to the x86 instruction set architecture for microprocessors from Intel and AMD proposed by Intel in March 2008 and first supported by Intel with the Sandy Bridge[1] processor shipping in Q1 2011 and later on by AMD with the Bulldozer[2] processor shipping in Q3 2011. AVX provides new features, new instructions and a new coding scheme.


### Tech

bitFlip uses a number of different approaches to flip those bits

* [Assembly] - win32 / elf. Compile with nasm - this routine is the fastest - just
* [Intel Implicit AVX] - using the handy gcc routines
* [Naive approaches] - A fun way to visualize a solution (also nice and terse)
* [Lambda Syntax] - just gotta try it out
* [Scala equivalent] - Look at the sveltness of the code - and the tardiness.

### Installation

bitFlip has both a cmake installer and makefiles.
The cmake installation will detect if you require google benchmark and gtest and will download and installed if required
```
mkdir cmake-build-release
cmake .. -G 'MSYS Makefiles'
make
```

##### Command line utility

```
tests/./flipbits --help
```

##### Benchmark/Tests

```
tests/./TestApp
```

##### Just for fun - Scala
Prints out bit flipped integers (as large as BigInt) from a range [FROM] [TO]

to run:
```
sbt console
run [FROM] [TO]


### Results

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


### Development

Please feel free...

License
----

MIT


[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)


   [wiki]: <https://en.wikipedia.org/wiki/Advanced_Vector_Extensions>


