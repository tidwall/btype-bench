#include <stdio.h>
#include "testutils.h"


int N = 1000000;
int R = 50;

#define BGEN_NAME      kv_int32
#define BGEN_TYPE      int32_t
#define BGEN_MALLOC    return malloc(size);
#define BGEN_FREE      free(ptr);
#define BGEN_NOATOMIC
#define BGEN_NOPATHHINT
#define BGEN_FANOUT 32
#define BGEN_COMPARE      { return a < b ? -1 : a > b; }
// #define BGEN_BSEARCH
// #define BGEN_LESS      { return a < b; }
#include "bgen.h"

#define BGEN_NAME      kv_uint64
#define BGEN_TYPE      uint64_t
#define BGEN_MALLOC    return malloc(size);
#define BGEN_FREE      free(ptr);
#define BGEN_NOATOMIC
#define BGEN_NOPATHHINT
#define BGEN_FANOUT 32
#define BGEN_COMPARE      { return a < b ? -1 : a > b; }
// #define BGEN_BSEARCH
// #define BGEN_LESS      { return a < b; }
#include "bgen.h"


#define BGEN_NAME      kv_string
#define BGEN_TYPE      char*
#define BGEN_MALLOC    return malloc(size);
#define BGEN_FREE      free(ptr);
#define BGEN_ITEMFREE  free(item);
#define BGEN_NOATOMIC
#define BGEN_NOPATHHINT
#define BGEN_FANOUT 32
#define BGEN_COMPARE      { return strcmp(a, b); }
// #define BGEN_LESS      { return a < b; }
#define BGEN_BSEARCH
#include "bgen.h"

static int compare_int32(const void *a, const void *b) {
    return *(int32_t*)a < *(int32_t*)b ? -1 : *(int32_t*)a > *(int32_t*)b;
}
static int compare_uint64(const void *a, const void *b) {
    return *(uint64_t*)a < *(uint64_t*)b ? -1 : *(uint64_t*)a > *(uint64_t*)b;
}
static int compare_string(const void *a, const void *b) {
    return strcmp(a, b);
}
static int compare_double(const void *a, const void *b) {
    return *(double*)a < *(double*)b ? -1 : *(double*)a > *(double*)b;
}


#define run_op(label, nruns, preop, op) {\
    double *telapsed = malloc(sizeof(double)*nruns); \
    for (int r = 0; r < (nruns); r++) { \
        printf("\r%-20s", label); \
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


#define free_keys(name, type, kfree) { \
    if (keys) { \
        for (int i = 0; i < N; i++) { \
            kfree \
        } \
        free(keys); keys = 0; \
    } \
}

#define fill_keys(name, type) { \
    int *perm = (int*)malloc(N * sizeof(int)); \
    for (int i = 0; i < N; i++) { \
        perm[i] = i; \
    } \
    shuffle0(perm, N, sizeof(int)); \
    free(keys); keys = 0; \
    keys = malloc(N * sizeof(type)); \
    assert(keys); \
    if (strcmp(#name,"string")==0) { \
        char buf[20]; \
        for (int i = 0; i < N; i++) { \
            int j = perm[i]; \
            snprintf(buf, 20, "%d", j); \
            keys[i] = (type)(uintptr_t)malloc(strlen(buf)+1); \
            assert(keys[i]); \
            strcpy((char*)(uintptr_t)keys[i], buf); \
        } \
    } else { \
        for (int i = 0; i < N; i++) { \
            int j = perm[i]; \
            keys[i] = (type)(uintptr_t)j; \
        } \
    } \
    free(perm); \
}

#define bench(name, type, kfree) { \
    printf("Benchmarking %d items, %s keys, %d times, using %s\n", \
        N, #name, R, "median"); \
    type *keys = 0; \
    struct kv_##name *tree = 0; \
    type val; \
    /* keys get allocated for each run and ownership is transfered to btree */ \
    run_op("insert(seq)", R, { \
        kv_##name##_clear(&tree, 0); \
        fill_keys(name, type); \
        qsort(keys, N, sizeof(type), compare_##name); \
    },{ \
        for (int i = 0; i < N; i++) { \
            kv_##name##_insert(&tree, keys[i], &val, 0); \
        } \
    }); \
    free(keys); keys = 0; \
    /* keys get allocated for each run and ownership is transfered to btree */ \
    run_op("insert(rand)", R, { \
        kv_##name##_clear(&tree, 0); \
        fill_keys(name, type); \
        shuffle0(keys, N, sizeof(type)); \
    },{ \
        for (int i = 0; i < N; i++) { \
            kv_##name##_insert(&tree, keys[i], &val, 0); \
        } \
    }); \
    free(keys); keys = 0; \
    /* keys get allocated for each run */ \
    run_op("get(seq)", R, { \
        free_keys(name, type, kfree); \
        fill_keys(name, type); \
        qsort(keys, N, sizeof(type), compare_##name); \
    }, { \
        for (int i = 0; i < N; i++) { \
            kv_##name##_get(&tree, keys[i], &val, 0); \
        } \
    }); \
    /* keys get allocated for each run */ \
    run_op("get(rand)", R, { \
        free_keys(name, type, kfree); \
        fill_keys(name, type); \
        shuffle0(keys, N, sizeof(type)); \
    }, { \
        for (int i = 0; i < N; i++) { \
            kv_##name##_get(&tree, keys[i], &val, 0); \
        } \
    }); \
    kv_##name##_clear(&tree, 0); \
    free_keys(name, type, kfree); \
}


int main(void) {
    if (getenv("N")) {
        N = atoi(getenv("N"));
    }
    if (getenv("R")) {
        R = atoi(getenv("R"));
    }
    seedrand();

    bench(int32, int32_t, {})
    bench(uint64, uint64_t, {})
    bench(string, char*, {free(keys[i]);} )

    return 0;
}
