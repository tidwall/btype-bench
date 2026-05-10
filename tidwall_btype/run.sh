#!/bin/bash

set -e
cd $(dirname "${BASH_SOURCE[0]}")

echo
printf "\e[0;32mtidwall/btype\e[0m (Go, btype.Map)\n"
go run main.go
