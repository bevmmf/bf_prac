#include "bloom.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "xxhash.h"              

#define XXH_INLINE_ALL     

// hash1 : XXHash3
static inline uint32_t hash32_xxhash3(uint32_t val, uint32_t seed)
{
    return XXH3_32bits_withSeed(&val, sizeof(val), seed);
}
// hash2 : FNV-1a
#define FNV_OFFSET_BASIS_32 0x811c9dc5u
#define FNV_PRIME_32        0x01000193u

static inline uint32_t hash32_fnv1a(uint32_t val, uint32_t seed)
{
    uint32_t h = FNV_OFFSET_BASIS_32 ^ seed;   /* 把 seed 混進雜湊 */
    for (int i = 0; i < 4; i++) {
        h ^= (uint8_t)(val & 0xFF);            /* 逐位元組處理 */
        h *= FNV_PRIME_32;
        val >>= 8;
    }
    return h;
}



int bloom_init(bf_t* bf, uint32_t m, uint32_t n, uint32_t k){
    bf->m = m;
    bf->n = n;
    bf->k = k;
    bf->bits_len = (m + 7) >> 8; // Calculate the length of the bit array in bytes
    bf->bits = calloc(bits_len,sizeof(uint8_t));
    if (bf->bits == NULL) {
        return -1; // Memory allocation failed
    }
    printf("Bloom filter initialized with m: %u, n: %u, k: %u\n", m, n, k);
    return 0; // Success    
}

int bloom_insert(bf_t* bf, int val){
    if (bf == NULL || bf->bits == NULL) {
        return -1; // Bloom filter not initialized
    }
    for(int i = 0; i < bf->k; i++){
        uint32_t hash_set[i] = hash32_xxhash3(val,0x01000193u+i);
        uint32_t bit_index = hash_set[i] % bf->m; 
        bf->bits[bf->m >> 3] |= (1 << (bit_index % 8)); // Set the bit in the bit array
    }
    printf("Inserted: %d\n", val);
    return 0; // Success
}


int bloom_query(bf_t* bf, int val){
    if (bf == NULL || bf->bits == NULL) {
        return -1; // Bloom filter not initialized
    }
    for(int i = 0; i < bf->k; i++){
        uint32_t hash_set[i] = hash32_xxhash3(val,0x01000193u+i);
        uint32_t bit_index = hash_set[i] % bf->m;
        // Check if the bit is set in the bit array
        if (!(bf->bits[bf->m >> 3] & (1 << (bit_index % 8)))) {
            printf("Not found: %d\n", val);
            return 0; // Not found
        }
    }
    printf("probably Found: %d\n", val);
    return 1; // Found
}


int bloom_destroy(bf_t* bf){
    if (bf == NULL) {
        return -1; // Bloom filter not initialized
    }
    free(bf->bits); // Free the bit array
    bf->bits = NULL; // Set pointer to NULL to avoid dangling pointer
    bf->m = 0;
    bf->n = 0;
    bf->k = 0;
    bf->bits_len = 0; 
    printf("Bloom filter freed\n");
    return 0; // Success
}