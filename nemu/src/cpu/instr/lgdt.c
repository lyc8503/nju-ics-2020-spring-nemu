#include "cpu/instr.h"

/*
Put the implementations of `lgdt' instructions here.
*/



make_instr_func(lgdt_instr) {

    OPERAND m;
    m.data_size = data_size;
    int len = modrm_rm(&m);

    operand_read(cpu.eip + 1, &m);

//    cpu.gdtr.limit = paddr_read(cpu.eip + 1, 2);
//    cpu.gdtr.base = paddr_read(cpu.eip + 3, 4);

    trace_instr("lgdt %x", m.val);

    return 1 + len;
}

