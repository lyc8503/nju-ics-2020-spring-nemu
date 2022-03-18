#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>

uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data)
{
	memcpy(hw_mem + paddr, &data, len);
}

uint32_t paddr_read(paddr_t paddr, size_t len) {
    uint32_t ret = 0;
#ifdef CACHE_ENABLED
    ret = cache_read(paddr, len);
#else
    ret = hw_mem_read(paddr, len);
#endif
    return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data) {
#ifdef CACHE_ENABLED
    cache_write(paddr, len, data);
#else
    hw_mem_write(paddr, len, data);
#endif
}

uint32_t laddr_read(laddr_t laddr, size_t len)
{
    if (cpu.cr0.pg || laddr > 0x7ffffff) {
        uint32_t offset_ = page_translate(laddr);
        return paddr_read(offset_, len);
    } else {
        return paddr_read(laddr, len);
    }
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data)
{
    if (cpu.cr0.pg || laddr > 0x7ffffff) {
        uint32_t offset_ = page_translate(laddr);
        paddr_write(offset_, len, data);
    } else {
        paddr_write(laddr, len, data);
    }
}

uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);

	printf("vaddr read %x\n", vaddr);
	if (cpu.cr0.pe) {
//	    printf("pe enabled read\n");
	    uint32_t offset_ = segment_translate(vaddr, sreg);
	    return laddr_read(offset_, len);
	} else {
	    return laddr_read(vaddr, len);
	}
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);

	printf("vaddr write %x %x\n", vaddr, data);

	if (cpu.cr0.pe) {
//	    printf("pe enabled write\n");
	    uint32_t offset_ = segment_translate(vaddr, sreg);
	    laddr_write(offset_, len, data);
	} else {
	    laddr_write(vaddr, len, data);
	}
}

void init_mem()
{
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);
	init_cache();

#ifdef TLB_ENABLED
	make_all_tlb();
	init_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t *get_mem_addr()
{
	return hw_mem;
}
