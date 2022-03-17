#include "cpu/instr.h"
/*
Put the implementations of `not' instructions here.
*/

static void instr_execute_1op()
{
    operand_read(&opr_src);
    opr_src.val = ~opr_src.val;
    operand_write(&opr_src);
}


make_instr_impl_1op(not, rm, v);
make_instr_impl_1op(not, rm, b);

