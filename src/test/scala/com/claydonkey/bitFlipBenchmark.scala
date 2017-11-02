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
  *         File:   main.scala
  *         Author: Anthony Campbell
  *
  *         Created on 05 October 2017, 11:13
  *
  */

import org.scalatest.FunSuite
import scala.collection.mutable.ListBuffer

package com.claydonkey {

  class bitFlipBenchmark extends FunSuite {
    //when used from org.scalatest use -> extends Bench.LocalTime {

    import org.scalameter._

    val times = ListBuffer[Quantity[Double]](Quantity[Double](1.0, ""))
    val iterations = 1000
    val kNoOfFlips = 10240 / 4
    val kIntSize = 64
    val rangeBI = BigInt(0L) to BigInt(kNoOfFlips)
    val rangeI = 0 to kNoOfFlips
    var r: Integer = 0
    val bits = 0 to kIntSize

    import org.scalameter._

    test("bitFlip inline benchmark with warming using String and BigInt") {
      val time = config(Key.exec.benchRuns -> iterations, Key.verbose -> false) withWarmer {
        new Warmer.Default
      } withMeasurer {
        new Measurer.IgnoringGC
      } measure {
        rangeBI.map((n) => BigInt(n.toString(2).reverse, 2))
      }
      val avg = time.value.doubleValue() / iterations
      println(s"[time] - String + BigInt Total time: $avg s")
    }

    test("bitFlip inline benchmark with warming Naive approach") {
      val time = config(Key.exec.benchRuns -> iterations, Key.verbose -> false) withWarmer {
        new Warmer.Default
      } withMeasurer {
        new Measurer.IgnoringGC
      } measure {
        rangeI.map((n) => {
          bits.fold(0)((l, r) => l | (((n >> r) & 1) << (kIntSize - r - 1)))
        })
      }
      val avg = time.value.doubleValue() / iterations
      println(s"[time] - Naive Total time: $avg s")
    }
  }run
}