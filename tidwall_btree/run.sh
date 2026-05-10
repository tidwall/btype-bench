#!/bin/bash

set -e
cd $(dirname "${BASH_SOURCE[0]}")

echo
printf "\e[0;32mtidwall/btree\e[0m (Go, btree.Map)\n"
go run main.go
