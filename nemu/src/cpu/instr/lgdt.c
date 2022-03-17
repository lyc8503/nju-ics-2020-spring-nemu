#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/

make_instr_func(lgdt_instr) {
    OPERAND m;
    m.data_size = data_size;
    int len = modrm_rm(eip + 1, &m);
    operand_read(&m);

    trace_instr("lgdt %x", m.val);

    return len + 1;
}

