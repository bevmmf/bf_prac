#include "bloom.h"

int main(){
    // Initialize a Bloom filter with m = 1000, n = 100, k = 3
    bf_t bloom_filter;
    if (bloom_init(&bloom_filter, 1000, 100, 3) != 0) {
        printf("Failed to initialize Bloom filter\n");
        return -1;
    }

    // Insert some values into the Bloom filter
    bloom_insert(&bloom_filter, 42);
    bloom_insert(&bloom_filter, 84);

    // Query the Bloom filter for inserted values
    bloom_query(&bloom_filter, 42); // Should return found
    bloom_query(&bloom_filter, 84); // Should return found
    bloom_query(&bloom_filter, 21); // Should return not found

    // Clean up the Bloom filter
    bloom_destroy(&bloom_filter);

    return 0;
}