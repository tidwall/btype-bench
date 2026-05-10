#!/bin/bash

set -e
cd $(dirname "${BASH_SOURCE[0]}")

echo
printf "\e[0;32mrust/btree\e[0m (Rust, std::collections::BTreeSet)\n"
cargo run --release --quiet
