#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
    PDE pde;
    printf("page translate laddr %x\n", laddr);
    pde.val = paddr_read(cpu.cr3.val + (laddr & 0xffe00000 >> 22) * 4, 4);
    printf("page translate pde %x\n", pde.val);
    assert(pde.present);
    PTE pte;
    pte.val = paddr_read(pde.page_frame * 4096 + (laddr & 0x1ff800 >> 11) * 4, 4);
    printf("page translate pte %x\n", pte.val);
    assert(pte.present);
    uint32_t paddr = pte.page_frame * 4096 + (laddr & 0x7ff);
    return paddr;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
