package main

import (
	"github.com/google/btree"
	"github.com/tidwall/btype-bench/tools/support"
)

func bench0[T support.KeyType]() {
	var b *btree.BTreeG[T]
	support.Bench(
		func() { b = btree.NewOrderedG[T](32) },
		func(key T) { support.AssertN2(b.ReplaceOrInsert(key)) },
		func(key T) { support.Assert2(b.Get(key)) },
	)
}

func main() {
	bench0[int32]()
	bench0[uint64]()
	bench0[string]()
}
