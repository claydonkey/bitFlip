/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bitFlipTest.h
 * Author: anthony
 *
 * Created on 09 October 2017, 19:46
 */

#ifndef BITFLIPTEST_H
#define BITFLIPTEST_H
#include <benchmark/benchmark.h>
#include <stdint.h>

void BM_bitFlip2(benchmark::State& state);
BENCHMARK(BM_bitFlip2);

void BM_bitFlip_Assym(benchmark::State& state);
BENCHMARK(BM_bitFlip_Assym);

void BM_naive(benchmark::State& state);
BENCHMARK(BM_naive);


void SetUp() ;
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif /* BITFLIPTEST_H */

