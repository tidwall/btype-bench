package main

import (
	"github.com/tidwall/btype"
	"github.com/tidwall/btype-bench/tools/support"
)

func bench0[T support.KeyType]() {
	var b *btype.Map[T, struct{}]
	support.Bench(
		func() { b = btype.NewMap[T, struct{}]() },
		func(key T) { support.Assert2(b.Insert(key, struct{}{})) },
		func(key T) { support.Assert2(b.Get(key)) },
	)
}

func main() {
	bench0[int32]()
	bench0[uint64]()
	bench0[string]()
}
