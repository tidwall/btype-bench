// c++ -Iinclude -std=c++20 bench.cpp

#include <fc/btree.h>
#include "bench.h"

int N = 1000000;
int R = 50;
const int M = 64;

static int compare_int32(const void *a, const void *b) {
    return *(int32_t*)a < *(int32_t*)b ? -1 : *(int32_t*)a > *(int32_t*)b;
}
static int compare_uint64(const void *a, const void *b) {
    return *(uint64_t*)a < *(uint64_t*)b ? -1 : *(uint64_t*)a > *(uint64_t*)b;
}
static int compare_string(const void *a, const void *b) {
    return *(double*)a < *(double*)b ? -1 : *(double*)a > *(double*)b;
    // const std::string* sa = static_cast<const std::string*>(a);
    // const std::string* sb = static_cast<const std::string*>(b);
    // return sa->compare(*sb); 
}
static int compare_double(const void *a, const void *b) {
    return *(double*)a < *(double*)b ? -1 : *(double*)a > *(double*)b;
}

#define run_op(label, nruns, preop, op) {\
    double *telapsed = (double*)malloc(sizeof(double)*nruns); \
    for (int r = 0; r < (nruns); r++) { \
        printf("\r%-19s", label); \
        printf("%d/%d ", r+1, (nruns)); \
        fflush(stdout); \
        preop \
        double start = now(); \
        op \
        telapsed[r] = now()-start; \
    } \
    printf("\r"); \
    printf("%-19s", label); \
    qsort(telapsed, nruns, sizeof(double), compare_double); \
    bench_print(N, 0, telapsed[nruns/2]); \
    free(telapsed); \
}(void)0


#define bench(name, keytype, type, less) { \
    printf("Benchmarking %d items, %s keys, %d times, using %s\n", \
        N, #name, R, "median"); \
    type *keys = new type[N]; \
    assert(keys); \
    if (strcmp(#name,"string")==0) { \
        char buf[20]; \
        for (int i = 0; i < N; i++) { \
            snprintf(buf, 20, "%d", i); \
            char *key = (char*)malloc(strlen(buf)+1); \
            strcpy(key, buf); \
            ((char**)keys)[i] = key; \
        } \
    } else { \
        for (int i = 0; i < N; i++) { \
            keys[i] = (type)(uintptr_t)i; \
        } \
    } \
    frozenca::BTreeSet<keytype, M less> tree;\
    run_op("insert(seq)", R, { \
        tree.clear(); \
        qsort(keys, N, sizeof(type), compare_##name); \
    },{ \
        for (int i = 0; i < N; i++) { \
            tree.insert(keys[i]); \
        } \
    }); \
    run_op("insert(rand)", R, { \
        tree.clear(); \
        shuffle0(keys, N, sizeof(type)); \
    },{ \
        for (int i = 0; i < N; i++) { \
            tree.insert(keys[i]); \
        } \
    }); \
    run_op("get(seq)", R, { \
        qsort(keys, N, sizeof(type), compare_##name); \
    },{ \
        for (int i = 0; i < N; i++) { \
            assert(tree.contains(keys[i])); \
        } \
    }); \
    run_op("get(rand)", R, { \
        shuffle0(keys, N, sizeof(type)); \
    },{ \
        for (int i = 0; i < N; i++) { \
            assert(tree.contains(keys[i])); \
        } \
    }); \
    delete[] keys; \
}

struct cstr_less {
  bool operator()(const char* a, const char* b) const noexcept {
    return std::strcmp(a, b) < 0;
  }
};

#define COMMA ,
#define NOTHING

int main(void) {
    if (getenv("N")) {
        N = atoi(getenv("N"));
    }
    if (getenv("R")) {
        R = atoi(getenv("R"));
    }
    seedrand();

    bench(int32, int32_t, int32_t, NOTHING)
    bench(uint64, uint64_t, uint64_t, NOTHING)
    bench(string, char*, char*, COMMA cstr_less)

    return 0;
}
