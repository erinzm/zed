#!/bin/bash
set -e # Error on fail

cat tests/testdata/helloworld.zd | ./zed
