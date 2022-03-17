#include "cpu/instr.h"
/*
Put the implementations of `adc' instructions here.
*/

#include "cpu/alu.h"

static void instr_execute_2op() {
    operand_read(&opr_src);
    operand_read(&opr_dest);
    //    trace_instr("adc src %x dest %x", opr_src.val, opr_dest.val);

    if (opr_src.data_size == 8 && (opr_src.val & 0x80)) {
        opr_src.val |= 0xffffff00;
    }

    opr_dest.val = alu_adc(opr_src.val, opr_dest.val, opr_dest.data_size);
    operand_write(&opr_dest);
}


make_instr_impl_2op(adc, r, rm, b)
make_instr_impl_2op(adc, r, rm, v)
make_instr_impl_2op(adc, rm, r, b)
make_instr_impl_2op(adc, rm, r, v)
make_instr_impl_2op(adc, i, a, b)
make_instr_impl_2op(adc, i, a, v)
make_instr_impl_2op(adc, i, rm, b)
make_instr_impl_2op(adc, i, rm, v)
make_instr_impl_2op(adc, i, rm, bv)
