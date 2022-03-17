#include "cpu/instr.h"
/*
Put the implementations of `and' instructions here.
*/

static void instr_execute_2op()
{
    operand_read(&opr_src);
    operand_read(&opr_dest);

    if (opr_src.data_size == 8 && (opr_src.val & 0x80)) {
        opr_src.val |= 0xffffff00;
    }

    opr_dest.val = alu_and(opr_src.val, opr_dest.val, opr_dest.data_size);
    operand_write(&opr_dest);
}

make_instr_impl_2op(and, r, rm, b)
make_instr_impl_2op(and, r, rm, v)
make_instr_impl_2op(and, rm, r, b)
make_instr_impl_2op(and, rm, r, v)
make_instr_impl_2op(and, i, a, b)
make_instr_impl_2op(and, i, a, v)
make_instr_impl_2op(and, i, rm, b)
make_instr_impl_2op(and, i, rm, v)
make_instr_impl_2op(and, i, rm, bv)
