#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/

make_instr_func(call_instr) {
    uint32_t rel = paddr_read(eip + 1, data_size / 8);

//    trace_instr("call data_size %d rel %x", data_size, rel);

    cpu.esp -= 4;
    paddr_write(cpu.esp, 4, cpu.eip + 1 + data_size / 8);
    cpu.eip = (cpu.eip + 1 + data_size / 8 + rel);
    return 0;
}


make_instr_func(call_instr_indirect) {

    OPERAND rm;
    rm.data_size = data_size;
    int len = modrm_rm(eip + 1, &rm);
    operand_read(&rm);

    cpu.esp -= 4;
    paddr_write(cpu.esp, 4, cpu.eip + 1 + len);
    cpu.eip = rm.val;
    return 0;
}
