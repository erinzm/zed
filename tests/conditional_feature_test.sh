#!/bin/bash
set -e # Error on fail

./zedc tests/testdata/conditional.zd
gcc tests/testdata/conditional.zd.ir.c
rm a.out
