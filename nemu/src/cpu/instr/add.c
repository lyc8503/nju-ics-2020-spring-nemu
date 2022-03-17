#include "cpu/instr.h"
/*
Put the implementations of `add' instructions here.
*/

#include "cpu/alu.h"

static void instr_execute_2op() {
    operand_read(&opr_src);
    operand_read(&opr_dest);
//    trace_instr("add src %x dest %x", opr_src.val, opr_dest.val);
    if (opr_src.data_size == 8 && (opr_src.val & 0x80)) {
//        trace_instr("add sign_ext src %x", opr_src.val);
        opr_src.val |= 0xffffff00;
    }

    opr_dest.val = alu_add(opr_src.val, opr_dest.val, opr_dest.data_size);
    operand_write(&opr_dest);
}


make_instr_impl_2op(add, r, rm, b)
make_instr_impl_2op(add, r, rm, v)
make_instr_impl_2op(add, rm, r, b)
make_instr_impl_2op(add, rm, r, v)
make_instr_impl_2op(add, i, a, b)
make_instr_impl_2op(add, i, a, v)
make_instr_impl_2op(add, i, rm, b)
make_instr_impl_2op(add, i, rm, v)
make_instr_impl_2op(add, i, rm, bv)
