#!/bin/bash
set -e # Error on fail

./zedc tests/testdata/helloworld.zd
gcc tests/testdata/helloworld.zd.ir.c
rm a.out
