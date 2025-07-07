#include <stddef.h>
#include <stdint.h>

#ifndef BLOOM_H
#define BLOOM_H




typedef struct bloom_t {
    uint32_t m;
    uint32_t n;
    uint32_t k;
    //bit array (practical data)
    size_t bits_len; // length of the bit array in bytes
    uint8_t* bits; // pointer to the bit array
    uint32_t seed; // seed for hash functions
} bf_t;

int bloom_init(bf_t* bf, uint32_t m, uint32_t n, uint32_t k);

int bloom_insert(bf_t* bf, int val);

int bloom_query(bf_t* bf, int val);

int bloom_destroy(bf_t* bf);

#endif