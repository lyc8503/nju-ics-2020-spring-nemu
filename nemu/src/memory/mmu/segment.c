#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	return cpu.segReg[sreg].base + offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	SegReg *reg = &cpu.segReg[sreg];
	SegDesc seg;

	seg.val[0] = paddr_read(cpu.gdtr.base + reg->index * 8, 4);
	seg.val[1] = paddr_read(cpu.gdtr.base + reg->index * 8 + 4, 4);

//	printf("load sreg data %08x %08x\n", seg.val[0], seg.val[1]);

	reg->base = seg.base_15_0 | (seg.base_23_16 << 16) | (seg.base_31_24) << 24;
	reg->limit = seg.limit_15_0 | (seg.limit_19_16) << 16;
	reg->type = seg.type;
	reg->privilege_level = seg.privilege_level;
	reg->soft_use = seg.soft_use;

//	printf("load sreg base %d limit %d granu %d \n", reg->base, reg->limit, seg.granularity);

	assert(reg->base == 0 && reg->limit == 0xfffff && seg.granularity == 1);
}
