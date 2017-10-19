package com.claydonkey

object main extends App {
    val x = Option(args).getOrElse(Array[String]("100"))
    (0 to x(0).toInt).map((n) => (println(BigInt(n.toBinaryString.reverse, 2))))
  }

