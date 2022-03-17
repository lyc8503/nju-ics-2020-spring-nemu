#include "cpu/instr.h"

/*
Put the implementations of `lgdt' instructions here.
*/



make_instr_func(lgdt_instr) {

//    OPERAND m;
//    m.data_size = data_size;
//    int len = modrm_rm(cpu.eip + 1, &m);
//
//    operand_read(&m);

//    cpu.gdtr.limit = paddr_read(cpu.eip + 1, 2);
//    cpu.gdtr.base = paddr_read(cpu.eip + 3, 4);
    OPERAND m;

    m.data_size = 16;
    int len = 1;
    len += modrm_rm(eip + 1, &m);

    operand_read(&m);

    cpu.gdtr.limit = m.val;

    m.data_size = 32;
    m.addr += 2;

    operand_read(&m);

    cpu.gdtr.base = m.val;

    trace_instr("lgdt base %x limit %x", m.val, cpu.gdtr.limit);

    return 1 + len;
}

