package support

import (
	"fmt"
	"math/rand/v2"
	"os"
	"reflect"
	"runtime"
	"slices"
	"strconv"
	"time"
	"unsafe"
)

type KeyType interface {
	~int32 | ~uint64 | ~string
}

func shuffle[T any](rng *rand.Rand, slice []T) {
	for i := range slice {
		j := rng.IntN(i + 1)
		slice[i], slice[j] = slice[j], slice[i]
	}
}

func randKeys[T KeyType](rng *rand.Rand, N int) []T {
	var empty T
	var keys []T
	if _, ok := any(empty).(string); ok {
		strs := make([]string, N)
		for i, j := range rng.Perm(N) {
			strs[i] = strconv.Itoa(j)
		}
		keys = unsafe.Slice((*T)(unsafe.Pointer(unsafe.SliceData(strs))),
			len(strs))

	} else {
		keys = make([]T, N)
		for i, j := range rng.Perm(N) {
			keys[i] = T(j)
		}
	}
	return keys
}

func run(label string, nruns int, pre, op func(), done func(float64)) {
	runtime.GC()
	var felapsed []float64
	for g := 0; g < (nruns); g++ {
		fmt.Printf("\r%-20s", label)
		fmt.Printf("%d/%d ", g+1, (nruns))
		if pre != nil {
			pre()
		}
		start := time.Now()
		op()
		felapsed = append(felapsed, time.Since(start).Seconds())
	}
	fmt.Printf("\r")
	fmt.Printf("%-19s", label)
	slices.Sort(felapsed)
	done(felapsed[nruns/2])
}

func commaize(n int) string {
	s1, s2 := fmt.Sprintf("%d", n), ""
	for i, j := len(s1)-1, 0; i >= 0; i, j = i-1, j+1 {
		if j%3 == 0 && j != 0 {
			s2 = "," + s2
		}
		s2 = string(s1[i]) + s2
	}
	return s2
}

func writeOutput(N int, elapsed time.Duration) {
	fmt.Printf(" %s ops in %7.3f secs %7.1f ns/op %13s op/sec\n",
		commaize(N), elapsed.Seconds(),
		float64(elapsed)/float64(N),
		commaize(int(float64(N)/elapsed.Seconds())),
	)
}

func bench[T KeyType](clear func(), insert func(key T),
	get func(key T), R int, N int,
) {
	seed := uint64(time.Now().UnixNano())
	rng := rand.New(rand.NewPCG(0, seed))
	var keys []T
	// new keys get allocated for each run
	run("insert(seq)", R, func() {
		clear()
		keys = randKeys[T](rng, N)
		slices.Sort(keys)
	}, func() {
		for i := range N {
			insert(keys[i])
		}
	}, func(elapsed float64) {
		writeOutput(N, time.Duration(elapsed*float64(time.Second)))
	})
	// new keys get allocated for each run
	run("insert(rand)", R, func() {
		clear()
		keys = randKeys[T](rng, N)
		shuffle(rng, keys)
	}, func() {
		for i := range N {
			insert(keys[i])
		}
	}, func(elapsed float64) {
		writeOutput(N, time.Duration(elapsed*float64(time.Second)))
	})
	// new keys get allocated for each run
	run("get(seq)", R, func() {
		keys = randKeys[T](rng, N)
		slices.Sort(keys)
	}, func() {
		for i := range N {
			get(keys[i])
		}
	}, func(elapsed float64) {
		writeOutput(N, time.Duration(elapsed*float64(time.Second)))
	})
	// new keys get allocated for each run
	run("get(rand)", R, func() {
		keys = randKeys[T](rng, N)
		shuffle(rng, keys)
	}, func() {
		for i := range N {
			get(keys[i])
		}
	}, func(elapsed float64) {
		writeOutput(N, time.Duration(elapsed*float64(time.Second)))
	})
}

func Assert(cond bool) {
	if !cond {
		panic("assertion failed")
	}
}

func Assert2(x any, cond bool) {
	if !cond {
		panic("assertion failed")
	}
}

func AssertN2(x any, cond bool) {
	if cond {
		panic("assertion failed")
	}
}

func Bench[T KeyType](clear func(), insert func(key T), get func(key T),
) {
	R, err := strconv.Atoi(os.Getenv("R"))
	if err != nil {
		R = 50
	}
	N, err := strconv.Atoi(os.Getenv("N"))
	if err != nil {
		N = 1000000
	}
	runtime.GOMAXPROCS(1)
	fmt.Printf("Benchmarking %d items, %s keys, %d times, using %s\n",
		N, reflect.TypeFor[T]().Name(), R, "median")
	bench(clear, insert, get, R, N)
}
