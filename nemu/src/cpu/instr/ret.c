#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.
*/

make_instr_func(ret_instr) {
    cpu.eip = paddr_read(cpu.esp, data_size / 8);
    cpu.esp += data_size / 8;
    return 0;
}

make_instr_func(ret_instr_imm16) {
    uint16_t imm = paddr_read(cpu.eip, 2);
    cpu.eip = paddr_read(cpu.esp, data_size / 8);
    cpu.esp += (data_size / 8 + imm);
    return 0;
}