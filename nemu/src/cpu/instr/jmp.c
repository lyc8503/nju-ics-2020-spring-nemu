#include "cpu/instr.h"

make_instr_func(jmp_near)
{
//    trace_instr("jmp near data_size %d", data_size);
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);

    int offset = sign_ext(rel.val, data_size);
    // thank Ting Xu from CS'17 for finding this bug
    print_asm_1("jmp", "", 1 + data_size / 8, &rel);

    cpu.eip += offset;

    return 1 + data_size / 8;
}


make_instr_func(jmp_short) {
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = 8;
    rel.addr = eip + 1;

    operand_read(&rel);
//    trace_instr("jmp short %d", (int8_t) rel.val);

    cpu.eip += (int8_t) rel.val;

    return 2;
}

make_instr_func(jmp_near_indirect) {
    OPERAND rm;
    rm.data_size = data_size;
    modrm_rm(eip + 1, &rm);
    operand_read(&rm);

//    trace_instr("jmp near indirect %x", rm.val);
    cpu.eip = rm.val;
    return 0;
}

make_instr_func(jmp_far_imm) {

    cpu.segReg.cs.val = paddr_read(eip + 5, 2);
    cpu.eip = paddr_read(eip + 1, 4);

    load_sreg(1);
    return 0;
}
