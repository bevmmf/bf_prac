#include "bloom.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h> 
#include <time.h> // For seeding the random number generator
#define XXH_STATIC_LINKING_ONLY   /* ← 告訴 header 要開 XXH3 Prototype */  
#include "xxhash.h"              



// hash1 : XXHash3
static inline uint32_t hash32_xxhash3(uint32_t val, uint32_t seed)
{
    return XXH32(&val, sizeof(val), seed);
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
    bf->bits_len = (m + 7) >> 3; // Calculate the length of the bit array in bytes
    bf->bits = calloc(bf->bits_len, 1);
    if (bf->bits == NULL) {
        return -1; // Memory allocation failed
    }
    srand((unsigned int)time(NULL));
    bf->seed = rand(); // Initialize the seed for hash functions
    printf("Bloom filter initialized with m: %u, n: %u, k: %u\n", m, n, k);
    return 0; // Success    
}


int bloom_insert(bf_t* bf, int val){
    if (bf == NULL || bf->bits == NULL) {
        return -1; // Bloom filter not initialized
    }
    for(int i = 0; i < bf->k; i++){
        uint32_t hash = hash32_xxhash3(val, bf->seed + i);
        uint32_t bit_index = hash % bf->m; 
        bf->bits[bit_index >> 3] |= (1 << (bit_index & 7)); // Set the bit in the bit array
    }
    printf("Inserted: %d\n", val);
    return 0; // Success
}


int bloom_query(bf_t* bf, int val){
    if (bf == NULL || bf->bits == NULL) {
        return -1; // Bloom filter not initialized
    }
    for(int i = 0; i < bf->k; i++){
        uint32_t hash = hash32_xxhash3(val, bf->seed + i);
        uint32_t bit_index = hash % bf->m;
        // Check if the bit is set in the bit array
        if (!(bf->bits[bit_index >> 3] & (1 << (bit_index & 7)))) {
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