#!/bin/bash

set -e
cd $(dirname "${BASH_SOURCE[0]}")


if [[ "$CXX" == "" ]]; then
    export CXX=c++
fi

if [[ "$CC" == "" ]]; then
    export CC=cc
fi

if [[ "$N" == "" ]]; then 
    export N=1000000
fi

run() {

    if [[ "`uname -s`" == "Linux" ]]; then
        echo "CPU: `cat /proc/cpuinfo | grep -m1 "model name" | cut -d' ' -f4-`"
    fi

    echo "Go: `go version`"
    echo "Rust: `rustc --version`"
    echo "C: `$CC --version | grep -m1 ""`"
    echo "C++: `$CXX --version | grep -m1 ""`"

    tidwall_btype/run.sh
    tidwall_btree/run.sh
    google_btree/run.sh
    rust_btree/run.sh
    tidwall_bgen/run.sh
    frozenca_btree/run.sh
}

run | tee >(sed $'s/\033[[][^A-Za-z]*[A-Za-z]//g' > out.txt)
