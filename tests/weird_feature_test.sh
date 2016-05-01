#!/bin/bash
set -e # Error on fail

./zedc tests/testdata/weird.zd
gcc tests/testdata/weird.zd.ir.c
rm a.out
