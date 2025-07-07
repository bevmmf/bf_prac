#include "bloom.h"
#include <stdio.h>

int main(){
    // Initialize a Bloom filter with m = 1000, n = 100, k = 3
    bf_t bloom_filter;
    if (bloom_init(&bloom_filter, 1000, 100, 3) != 0) {
        printf("Failed to initialize Bloom filter\n");
        return -1;
    }

    // Insert values into the Bloom filter
    for(int i = 0; i < 100; i++) {
        bloom_insert(&bloom_filter, i);
    }

    // Query 100 times and solve FPR
    int count = 0;
    int test_nr = 1000;
    for(int i = 100; i < 100 + test_nr; i++) {
        if (bloom_query(&bloom_filter, i) == 1)
            count++;
    }
    printf("FPR : %4f\n", (float)count / test_nr);

    // Clean up the Bloom filter
    bloom_destroy(&bloom_filter);

    return 0;
}