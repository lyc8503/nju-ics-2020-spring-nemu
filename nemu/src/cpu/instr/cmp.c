#include "cpu/instr.h"

#include "cpu/alu.h"
/*
Put the implementations of `cmp' instructions here.
*/

// cmp
static void instr_execute_2op() {
    operand_read(&opr_src);
    operand_read(&opr_dest);

    if (opr_src.data_size == 8 && (opr_src.val & 0x80) && opr_dest.data_size > 8) {
        if (opr_dest.data_size == 16) {
            opr_src.val |= 0xff00;
        } else {
            opr_src.val |= 0xffffff00;
        }
    }

    alu_sub(opr_src.val, opr_dest.val, data_size);
}

make_instr_impl_2op(cmp, r, rm, b)
make_instr_impl_2op(cmp, r, rm, v)
make_instr_impl_2op(cmp, rm, r, b)
make_instr_impl_2op(cmp, rm, r, v)
make_instr_impl_2op(cmp, i, a, b)
make_instr_impl_2op(cmp, i, a, v)
make_instr_impl_2op(cmp, i, rm, b)
make_instr_impl_2op(cmp, i, rm, v)
make_instr_impl_2op(cmp, i, rm, bv)
