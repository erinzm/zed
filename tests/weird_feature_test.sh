#!/bin/bash
set -e # Error on fail

cat tests/testdata/weird.ciaw | ./ciaw
