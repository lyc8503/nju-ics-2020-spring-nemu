#include "cpu/instr.h"

/*
Put the implementations of `lgdt' instructions here.
*/



make_instr_func(lgdt_instr) {

    cpu.gdtr.limit = paddr_read(cpu.eip + 1, 2);
    cpu.gdtr.base = paddr_read(cpu.eip + 3, 4);

    trace_instr("limit %x base %x", cpu.gdtr.limit, cpu.gdtr.base);

    return 6;
}

