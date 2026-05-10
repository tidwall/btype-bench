# btype benchmark

Compares various state-of-the-art B-tree implementations.

- [tidwall/btype](https://github.com/tidwall/btype) Go
- [google/btree](https://github.com/google/btree) Go
- [tidwall/btree](https://github.com/tidwall/btree) Go
- [rust/BTreeMap](https://doc.rust-lang.org/std/collections/struct.BTreeMap.html) Rust
- [tidwall/bgen](https://github.com/tidwall/bgen) C
- [frozenca/btree](https://github.com/frozenca/BTree) C++

### Benchmarks

- CPU: Ryzen 9 5950X 16-Core Processor
- Go: go version go1.26.0 linux/amd64
- Rust: rustc 1.95.0 (59807616e 2026-04-14)
- C: gcc (Ubuntu 11.4.0-1ubuntu1~22.04.3) 11.4.0
- C++: g++ (Ubuntu 11.4.0-1ubuntu1~22.04.3) 11.4.0

Benchmarking 1,000,000 items, 50 runs, taking the average result.

### int32 keys

```
tidwall/btype
insert(seq)      1,000,000 ops in   0.017 secs   17.1 ns/op   58,389,383 op/sec
insert(rand)     1,000,000 ops in   0.076 secs   76.0 ns/op   13,159,734 op/sec
get(seq)         1,000,000 ops in   0.030 secs   29.8 ns/op   33,599,978 op/sec
get(rand)        1,000,000 ops in   0.064 secs   64.4 ns/op   15,535,343 op/sec

tidwall/btree
insert(seq)      1,000,000 ops in   0.037 secs   36.9 ns/op   27,104,762 op/sec
insert(rand)     1,000,000 ops in   0.134 secs  134.3 ns/op    7,445,864 op/sec
get(seq)         1,000,000 ops in   0.041 secs   41.4 ns/op   24,141,559 op/sec
get(rand)        1,000,000 ops in   0.128 secs  127.9 ns/op    7,817,200 op/sec

google/btree
insert(seq)      1,000,000 ops in   0.070 secs   69.8 ns/op   14,321,709 op/sec
insert(rand)     1,000,000 ops in   0.153 secs  153.4 ns/op    6,518,280 op/sec
get(seq)         1,000,000 ops in   0.065 secs   64.6 ns/op   15,486,010 op/sec
get(rand)        1,000,000 ops in   0.155 secs  154.9 ns/op    6,454,916 op/sec

rust/btree
insert(seq)      1,000,000 ops in   0.051 secs   51.0 ns/op   19,624,389 op/sec
insert(rand)     1,000,000 ops in   0.098 secs   98.2 ns/op   10,187,241 op/sec
get(seq)         1,000,000 ops in   0.033 secs   32.6 ns/op   30,650,401 op/sec
get(rand)        1,000,000 ops in   0.097 secs   97.0 ns/op   10,308,321 op/sec

tidwall/bgen
insert(seq)      1,000,000 ops in   0.053 secs   52.6 ns/op   19,011,130 op/sec
insert(rand)     1,000,000 ops in   0.076 secs   75.8 ns/op   13,186,500 op/sec
get(seq)         1,000,000 ops in   0.033 secs   33.0 ns/op   30,264,262 op/sec
get(rand)        1,000,000 ops in   0.069 secs   68.9 ns/op   14,524,288 op/sec

frozenca/btree
insert(seq)      1,000,000 ops in   0.093 secs   92.7 ns/op   10,782,702 op/sec
insert(rand)     1,000,000 ops in   0.081 secs   81.5 ns/op   12,275,940 op/sec
get(seq)         1,000,000 ops in   0.044 secs   44.2 ns/op   22,636,693 op/sec
get(rand)        1,000,000 ops in   0.079 secs   79.1 ns/op   12,638,367 op/sec
```

### uint64 keys

```
tidwall/btype
insert(seq)      1,000,000 ops in   0.018 secs   18.3 ns/op   54,498,410 op/sec
insert(rand)     1,000,000 ops in   0.080 secs   80.3 ns/op   12,457,898 op/sec
get(seq)         1,000,000 ops in   0.030 secs   30.1 ns/op   33,200,724 op/sec
get(rand)        1,000,000 ops in   0.072 secs   72.4 ns/op   13,819,949 op/sec

tidwall/btree
insert(seq)      1,000,000 ops in   0.039 secs   38.7 ns/op   25,824,082 op/sec
insert(rand)     1,000,000 ops in   0.146 secs  146.0 ns/op    6,849,574 op/sec
get(seq)         1,000,000 ops in   0.053 secs   52.6 ns/op   19,010,440 op/sec
get(rand)        1,000,000 ops in   0.141 secs  140.9 ns/op    7,099,716 op/sec

google/btree
insert(seq)      1,000,000 ops in   0.077 secs   76.8 ns/op   13,028,686 op/sec
insert(rand)     1,000,000 ops in   0.173 secs  172.9 ns/op    5,784,271 op/sec
get(seq)         1,000,000 ops in   0.062 secs   61.9 ns/op   16,165,628 op/sec
get(rand)        1,000,000 ops in   0.166 secs  166.4 ns/op    6,008,125 op/sec

rust/btree
insert(seq)      1,000,000 ops in   0.044 secs   43.6 ns/op   22,936,305 op/sec
insert(rand)     1,000,000 ops in   0.105 secs  105.2 ns/op    9,502,632 op/sec
get(seq)         1,000,000 ops in   0.034 secs   33.9 ns/op   29,509,841 op/sec
get(rand)        1,000,000 ops in   0.107 secs  106.8 ns/op    9,362,769 op/sec

tidwall/bgen
insert(seq)      1,000,000 ops in   0.054 secs   53.7 ns/op   18,605,963 op/sec
insert(rand)     1,000,000 ops in   0.081 secs   80.6 ns/op   12,406,050 op/sec
get(seq)         1,000,000 ops in   0.033 secs   32.6 ns/op   30,657,821 op/sec
get(rand)        1,000,000 ops in   0.075 secs   75.4 ns/op   13,269,668 op/sec

frozenca/btree
insert(seq)      1,000,000 ops in   0.094 secs   93.7 ns/op   10,668,965 op/sec
insert(rand)     1,000,000 ops in   0.094 secs   93.6 ns/op   10,688,826 op/sec
get(seq)         1,000,000 ops in   0.044 secs   43.5 ns/op   22,964,359 op/sec
get(rand)        1,000,000 ops in   0.087 secs   87.0 ns/op   11,497,710 op/sec
```

### string keys

```
tidwall/btype
insert(seq)      1,000,000 ops in   0.074 secs   73.9 ns/op   13,534,682 op/sec
insert(rand)     1,000,000 ops in   0.287 secs  287.2 ns/op    3,482,420 op/sec
get(seq)         1,000,000 ops in   0.094 secs   93.6 ns/op   10,683,081 op/sec
get(rand)        1,000,000 ops in   0.328 secs  327.6 ns/op    3,052,945 op/sec

tidwall/btree
insert(seq)      1,000,000 ops in   0.124 secs  124.3 ns/op    8,043,933 op/sec
insert(rand)     1,000,000 ops in   0.402 secs  402.3 ns/op    2,485,505 op/sec
get(seq)         1,000,000 ops in   0.122 secs  122.4 ns/op    8,171,636 op/sec
get(rand)        1,000,000 ops in   0.452 secs  452.0 ns/op    2,212,330 op/sec

google/btree
insert(seq)      1,000,000 ops in   0.191 secs  191.0 ns/op    5,234,886 op/sec
insert(rand)     1,000,000 ops in   0.437 secs  437.3 ns/op    2,286,980 op/sec
get(seq)         1,000,000 ops in   0.146 secs  145.9 ns/op    6,855,693 op/sec
get(rand)        1,000,000 ops in   0.487 secs  487.0 ns/op    2,053,473 op/sec

rust/btree
insert(seq)      1,000,000 ops in   0.250 secs  250.3 ns/op    3,995,141 op/sec
insert(rand)     1,000,000 ops in   0.510 secs  510.3 ns/op    1,959,504 op/sec
get(seq)         1,000,000 ops in   0.218 secs  218.3 ns/op    4,581,355 op/sec
get(rand)        1,000,000 ops in   0.591 secs  591.0 ns/op    1,692,138 op/sec

tidwall/bgen
insert(seq)      1,000,000 ops in   0.392 secs  392.3 ns/op    2,549,279 op/sec
insert(rand)     1,000,000 ops in   0.400 secs  400.5 ns/op    2,496,899 op/sec
get(seq)         1,000,000 ops in   0.466 secs  466.3 ns/op    2,144,526 op/sec
get(rand)        1,000,000 ops in   0.478 secs  477.6 ns/op    2,093,912 op/sec

frozenca/btree
insert(seq)      1,000,000 ops in   0.636 secs  636.3 ns/op    1,571,476 op/sec
insert(rand)     1,000,000 ops in   0.633 secs  632.7 ns/op    1,580,596 op/sec
get(seq)         1,000,000 ops in   0.376 secs  376.5 ns/op    2,656,345 op/sec
get(rand)        1,000,000 ops in   0.618 secs  618.4 ns/op    1,617,120 op/sec
```