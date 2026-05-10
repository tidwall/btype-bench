#!/bin/bash

set -e
cd $(dirname "${BASH_SOURCE[0]}")

if [[ "$CXX" == "" ]]; then
    CXX=c++
fi

echo
printf "\e[0;32mfrozenca/btree\e[0m (C++, frozenca::BTreeSet)\n"
$CXX -Iinclude -std=c++20 -O3 $CXXFLAGS bench.cpp && ./a.out; rm -f ./a.out
