#include "memory/mmu/cache.h"
#include "memory/memory.h"
#include <stdlib.h>

//#define DEBUG

//int counter = 0;
//int ori = 0;

#ifdef DEBUG
#define trace_cache(format, args...) do {                   \
printf("[CACHE:%d] " format "\n", __LINE__, ##args);        \
} while(0)
#else
#define trace_cache(format, args...) ((void)0)
#endif

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


uint32_t hw_mem_read_(paddr_t paddr, size_t len)
{
    uint32_t ret = 0;
    memcpy(&ret, hw_mem + paddr, len);
    return ret;
}

// init the cache
void init_cache() {
    for (int i = 0; i < CACHE_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            cache.blocks[i].lines[j].addr = 0xffffffff;
        }
    }
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data) {
    trace_cache("write paddr %x len %d data %x", paddr, len, data);

    assert(len == 1 || len == 2 || len == 4);

    uint32_t t = paddr / (128 * 1024 * 1024 / CACHE_SIZE);
    CACHE_BLOCK* b = &cache.blocks[t];

    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (paddr >= b->lines[i].addr && paddr + len < b->lines[i].addr + LINE_SIZE) {
            b->lines[i].addr = 0xffffffff;
        }
    }

    memcpy(hw_mem + paddr, &data, len);
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len) {

//    printf("before %d, after %d\n", ori, counter);

//    ori += 10;

    trace_cache("read paddr %x len %d", paddr, len);

    uint32_t t = paddr / (128 * 1024 * 1024 / CACHE_SIZE);
    CACHE_BLOCK* b = &cache.blocks[t];

    trace_cache("block %d", t);

    uint32_t hit_flag = 0;
    uint32_t ret = 0;

    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (paddr >= b->lines[i].addr && paddr + len < b->lines[i].addr + LINE_SIZE) {
//            counter += 1;
            trace_cache("cache hit");
            hit_flag = 1;
            memcpy(&ret, &b->lines[i].data[paddr - b->lines[i].addr], len);
            break;
        }
    }

    // cache miss
    if (!hit_flag) {
//        counter += 10;
        // cache
        int i = 0;
        for (; i < BLOCK_SIZE; i++) {
            if (b->lines[i].addr == 0xffffffff) {
                break;
            }
        }

        if (i == BLOCK_SIZE) {
            // full, replace random
            i = rand() % BLOCK_SIZE;
        }

        memcpy(b->lines[i].data, hw_mem + paddr, LINE_SIZE);
        b->lines[i].addr = paddr;
        trace_cache("not full %d %x", i, b->lines[i].addr);
        return cache_read(paddr, len);
    }

    trace_cache("read got %x expect %x", ret, hw_mem_read_(paddr, len));

    return ret;
}

