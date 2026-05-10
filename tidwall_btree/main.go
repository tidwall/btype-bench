package main

import (
	"github.com/tidwall/btree"
	"github.com/tidwall/btype-bench/tools/support"
)

func bench0[T support.KeyType]() {
	var b *btree.Map[T, struct{}]
	support.Bench(
		func() { b = btree.NewMap[T, struct{}](32) },
		func(key T) { support.AssertN2(b.Set(key, struct{}{})) },
		func(key T) { support.Assert2(b.Get(key)) },
	)
}

func main() {
	bench0[int32]()
	bench0[uint64]()
	bench0[string]()
}
