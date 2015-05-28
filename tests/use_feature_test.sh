#!/bin/bash
set -e # Error on fail

cat tests/testdata/use.ciaw | ./ciaw
