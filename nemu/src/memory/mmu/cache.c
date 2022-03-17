#include "memory/mmu/cache.h"


#define LINE_SIZE 8
#define BLOCK_SIZE 8
#define CACHE_SIZE 1024

typedef struct CACHE_LINE {
    union DATA {
        uint8_t data[LINE_SIZE];
        uint32_t data32[LINE_SIZE / 4];
    } d;
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
    uint32_t data = 0;

    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (paddr >= b.lines[i].addr && paddr + len < b.lines[i].addr + LINE_SIZE) {
            hit_flag = 1;
            for (int j = 0; j < len; j++) {
                data |= (b.lines[i].d.data[paddr - b.lines[i].addr] << (j - 1) * 8);
            }
            break;
        }
    }

    // cache miss
    if (!hit_flag) {
        // hw read
        uint32_t data = hw_mem_read(paddr, 4);
        uint32_t data2 = hw_mem_read(paddr + 1, 4);

        // cache
//        for (int i = 0; i < BLOCK_SIZE; i++) {
//            if (b.lines[i].addr == 0xffffffff) {
//                break;
//            }
//        }
//
//        if (i < BLOCK_SIZE) {
//            // not full
//            b.lines[i].addr == paddr;
//            b.lines[i].DATA32[0] == ;
//        } else {
//            // full
//
//        }
    }

	return data;
}

