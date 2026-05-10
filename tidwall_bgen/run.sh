#!/bin/bash

set -e
cd $(dirname "${BASH_SOURCE[0]}")

if [[ "$CC" == "" ]]; then
    CC=cc
fi

echo
printf "\e[0;32mtidwall/bgen\e[0m (C)\n"
$CC -O3 $CFLAGS bench_b.c && ./a.out; rm ./a.out
