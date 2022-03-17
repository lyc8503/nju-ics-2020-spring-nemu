#include "cpu/instr.h"
/*
Put the implementations of `lea' instructions here.
*/

make_instr_func(lea_instr) {
    OPERAND dest, src;

    dest.data_size = src.data_size = data_size;

    int len = modrm_r_rm(cpu.eip + 1, &dest, &src);
    dest.val = src.addr;

    operand_write(&dest);
    return 1 + len;
}