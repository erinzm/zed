#!/bin/bash
set -e # Error on fail

./zedc tests/testdata/variable.zd
gcc tests/testdata/variable.zd.ir.c
rm a.out
