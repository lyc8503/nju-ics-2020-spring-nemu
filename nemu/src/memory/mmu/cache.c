#include "memory/mmu/cache.h"

#define CACHE_BLOCK_SIZE 64
#define CACHE_BLOCK_NUM 1024

uint8_t l1cache[CACHE_BLOCK_NUM][CACHE_BLOCK_SIZE];
uint8_t valid_bits[CACHE_BLOCK_NUM];

// init the cache
void init_cache()
{
    memset(valid_bits, 0, CACHE_BLOCK_NUM / 8);
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{
	// implement me in PA 3-1
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{
	// implement me in PA 3-1
	return 0;
}

