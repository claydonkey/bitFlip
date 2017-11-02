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
package com.claydonkey

import scala.language.postfixOps

object main extends App {
  val arg = Option(args).getOrElse(Array[String]("0", "100"))
  val range = BigInt(arg(0)) to BigInt(arg(1))
  val rangeI = Integer.parseUnsignedInt(arg(0)) to Integer.parseUnsignedInt(arg(1))

  range.map((n) => println(n + "-->" + BigInt(n toString 2 reverse, 2)))

  range.map((n) => println(n + "-->" + (n toString 2 reverse)))

  val intSize = 32
  val bits = 0 to intSize
  //todo fix reduce fold ? to reproduce c naive version
  //
  rangeI.map((n) => {
    val b = BigInt(bits.reduce((l, r) => l | (((n >> r) & 1) << (intSize - r - 1))))
    val lsb = b.lowestSetBit
    val s = b.toString(2) //.stripPrefix("-")//.dropRight(lsb)
    println(n + "-->" + s)
    // println(n + "-->" + Integer.parseUnsignedInt(s, 2))
  })
}

