#!/bin/bash

set -e
cd $(dirname "${BASH_SOURCE[0]}")

echo
printf "\e[0;32mgoogle/btree\e[0m (Go, btree.BTreeG)\n"
go run main.go
