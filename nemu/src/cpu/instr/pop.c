#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/

static void instr_execute_1op()
{
    opr_src.val = paddr_read(cpu.esp, data_size / 8);
    cpu.esp += data_size / 8;

//    trace_instr("pop data_size %d, esp_old %x, data %x", data_size, cpu.esp, opr_src.val);

    operand_write(&opr_src);
}


make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop, rm, v)

