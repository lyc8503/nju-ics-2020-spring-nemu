#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/

static void instr_execute_1op()
{

    operand_read(&opr_src);
//    trace_instr("push data_size %d, esp_old %x, data %x", data_size, cpu.esp, opr_src.val);

    cpu.esp -= data_size / 8;
    paddr_write(cpu.esp, data_size / 8, opr_src.val);
}


make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)

make_instr_impl_1op(push, i, v)
make_instr_impl_1op(push, i, b)
