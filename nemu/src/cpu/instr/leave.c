#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/


make_instr_func(leave_instr) {
    cpu.esp = cpu.ebp;
    cpu.ebp = paddr_read(cpu.esp, 4);
    cpu.esp += 4;
    return 1;
}