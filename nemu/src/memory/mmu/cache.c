#include "memory/mmu/cache.h"
#include "memory/memory.h"

#define LINE_SIZE 8
#define BLOCK_SIZE 8
#define CACHE_SIZE 1024

typedef struct CACHE_LINE {
    uint8_t data[LINE_SIZE];
    uint32_t addr;
} CACHE_LINE;

typedef struct CACHE_BLOCK {
    CACHE_LINE lines[BLOCK_SIZE];
} CACHE_BLOCK;

typedef struct L1CACHE {
    CACHE_BLOCK blocks[CACHE_SIZE];
} L1CACHE;

L1CACHE cache;

// init the cache
void init_cache()
{
    for (int i = 0; i < CACHE_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            cache.blocks[i].lines[j].addr = 0xffffffff;
        }
    }
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{


	// implement me in PA 3-1
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{

    uint32_t t = paddr / (128 * 1024 * 1024 / CACHE_SIZE);
    CACHE_BLOCK b = cache.blocks[t];

    uint32_t hit_flag = 0;
    uint32_t ret = 0;

    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (paddr >= b.lines[i].addr && paddr + len < b.lines[i].addr + LINE_SIZE) {
            hit_flag = 1;
            switch (len) {
                case 4:
                    ret = *((uint32_t*) b.lines[i].d.data + paddr - b.lines[i].addr);
                    break;
                case 2:
                    ret = *((uint16_t*) b.lines[i].d.data + paddr - b.lines[i].addr);
                    break;
                case 1:
                    ret = *((uint8_t*) b.lines[i].d.data + paddr - b.lines[i].addr);
                    break;
                default:
                    assert(0);
            }
            break;
        }
    }

    // cache miss
    if (!hit_flag) {
        // cache
        int i = 0;
        for (; i < BLOCK_SIZE; i++) {
            if (b.lines[i].addr == 0xffffffff) {
                break;
            }
        }

        if (i < BLOCK_SIZE) {
            // not full
            memcpy(b.lines[i].data, hw_mem + paddr, 8);
            b.lines[i].addr = paddr;
        } else {
            // full
        }
    }

	return ret;
}

