#include "cpu/cpu.h"

#define DEBUG

#ifdef DEBUG
#define trace_alu(format, args...) do {                   \
    printf("[ALU:%d] " format "\n", __LINE__, ##args);    \
} while(0)
#else
#define trace_alu(format, args...) ((void)0)
#endif

inline void alu_set_flag_bits(uint32_t res, size_t data_size) {
    // set PF
    char counter = 0;
    for (int i = 0; i < 8; i++) {
        if (res & (1 << i)) {
            counter += 1;
        }
    }
    cpu.eflags.PF = (counter % 2 == 0);

    // set ZF
    cpu.eflags.ZF = (res == 0);

    // set SF-
    cpu.eflags.SF = (res & (1 << (data_size - 1))) > 0;
}


uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_add(src, dest, data_size);
#else

//    trace_alu("ORIGIN src %x, dest %x, size %d, and %x", src, dest, data_size, (uint32_t) 0xFFFFFFFF >> (32 - data_size));
    uint32_t mask = ((uint32_t) 0xFFFFFFFF >> (32 - data_size));
    src &= mask;
    dest &= mask;

    uint32_t res = (src + dest) & mask;

    alu_set_flag_bits(res, data_size);

//    trace_alu("src %x, dest %x, size %d, result %x", src, dest, data_size, res);
    // set CF
    cpu.eflags.CF = (res < src || res < dest);
    // set OF
    uint32_t bit1 = src & (1 << (data_size - 1));
    uint32_t bit2 = dest & (1 << (data_size - 1));
    uint32_t bit3 = res & (1 << (data_size - 1));
    cpu.eflags.OF = (bit1 && bit2 && !bit3) || (!bit1 && !bit2 && bit3);

    return res;
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_adc(src, dest, data_size);
#else
//    trace_alu("src %x, dest %x, size %d, CF %d", src, dest, data_size, cpu.eflags.CF);
    uint8_t cf_temp = cpu.eflags.CF;
    uint32_t mask = ((uint32_t) 0xFFFFFFFF >> (32 - data_size));

    src &= mask;
    dest &= mask;

    uint32_t res = (src + dest + cf_temp) & mask;

    alu_set_flag_bits(res, data_size);

    // set CF
    cpu.eflags.CF = (res < (uint64_t) src + cf_temp || res < (uint64_t) dest + cf_temp);
    // set OF
    uint32_t bit1 = src & (1 << (data_size - 1));
    uint32_t bit2 = dest & (1 << (data_size - 1));
    uint32_t bit3 = res & (1 << (data_size - 1));
    cpu.eflags.OF = (bit1 && bit2 && !bit3) || (!bit1 && !bit2 && bit3);

    return res;
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_sub(src, dest, data_size);
#else

//    trace_alu("ORIGIN src %x, dest %x, size %d", src, dest, data_size);

    uint32_t mask = ((uint32_t) 0xFFFFFFFF >> (32 - data_size));

    src &= mask;
    dest &= mask;

    uint32_t res = (dest - src) & mask;

    alu_set_flag_bits(res, data_size);

    // set CF
    cpu.eflags.CF = (dest < src);
    // set OF
    uint32_t msb1 = dest & (1 << (data_size - 1));
    uint32_t msb2 = src & (1 << (data_size - 1));
    uint32_t msb3 = res & (1 << (data_size - 1));
    cpu.eflags.OF = ((!msb1 && msb2 && msb3) || (msb1 && !msb2 && !msb3)) && !(dest == src);

    return res;

#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_sbb(src, dest, data_size);
#else

    uint32_t mask = ((uint32_t) 0xFFFFFFFF >> (32 - data_size));

    src &= mask;
    dest &= mask;
    uint8_t cf_temp = cpu.eflags.CF;

    uint32_t res = (dest - src - cf_temp) & mask;

    alu_set_flag_bits(res, data_size);

    // set CF
    cpu.eflags.CF = (dest < (uint64_t) src + cf_temp);
    // set OF
    uint32_t msb1 = dest & (1 << (data_size - 1));
    uint32_t msb2 = src & (1 << (data_size - 1));
    uint32_t msb3 = res & (1 << (data_size - 1));
    cpu.eflags.OF = ((!msb1 && msb2 && msb3) || (msb1 && !msb2 && !msb3)) && !(dest == src);

    return res;
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_mul(src, dest, data_size);
#else
//    trace_alu("ORIGIN src %x, dest %x, size %d", src, dest, data_size);

    uint32_t mask = ((uint32_t) 0xFFFFFFFF >> (32 - data_size));

    uint64_t src64 = src & mask;
    uint64_t dest64 = dest & mask;

    uint64_t res = src64 * dest64;

    // set CF and OF
    cpu.eflags.OF = cpu.eflags.CF = (res & (uint64_t) 0xFFFFFFFFFFFFFFFF << data_size) > 0;

    return res;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_imul(src, dest, data_size);
#else
//    trace_alu("ORIGIN src %x, dest %x, size %d", src, dest, data_size);

    switch (data_size) {
        case 8:
            return (int8_t) src * (int8_t) dest;
        case 16:
            return (int16_t) src * (int16_t) dest;
        case 32:
            // 64 is correct here
            return (int64_t) src * (int64_t) dest;
    }

    assert(0);
    return 0;
#endif
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_div(src, dest, data_size);
#else
    uint64_t mask64 = ((uint64_t) 0xFFFFFFFFFFFFFFFF >> (64 - data_size));
    src &= mask64;
    dest &= mask64;

    uint32_t res = dest / src;

    return res;
#endif
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_idiv(src, dest, data_size);
#else

//    trace_alu("ORIGIN src %llx, dest %llx, size %d", src, dest, data_size);

    switch (data_size) {
        case 8:
            return (int8_t) dest / (int8_t) src;
        case 16:
            return (int16_t) dest / (int16_t) src;
        case 32:
            return (int32_t) dest / (int32_t) src;
    }
    assert(0);
    return 0;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
#ifdef NEMU_REF_ALU
    return __ref_alu_mod(src, dest);
#else
    return dest % src;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest) {
#ifdef NEMU_REF_ALU
    return __ref_alu_imod(src, dest);
#else
    return dest % src;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_and(src, dest, data_size);
#else
    uint32_t res = src & dest & ((uint32_t) 0xFFFFFFFF >> (32 - data_size));
    alu_set_flag_bits(res, data_size);
//    cpu.eflags.CF = cpu.eflags.OF = 0;
    return res;
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_xor(src, dest, data_size);
#else
    uint32_t res = (src ^ dest) & ((uint32_t) 0xFFFFFFFF >> (32 - data_size));
    alu_set_flag_bits(res, data_size);
    cpu.eflags.CF = cpu.eflags.OF = 0;
    return res;
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_or(src, dest, data_size);
#else
    uint32_t res = (src | dest) & ((uint32_t) 0xFFFFFFFF >> (32 - data_size));
    alu_set_flag_bits(res, data_size);
    return res;
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_shl(src, dest, data_size);
#else
    return alu_sal(src, dest, data_size);
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_shr(src, dest, data_size);
#else
    uint32_t mask = ((uint32_t) 0xFFFFFFFF >> (32 - data_size));

    src &= mask;
    dest &= mask;

    uint32_t res = (dest >> src) & mask;

    alu_set_flag_bits(res, data_size);

    // set CF
    cpu.eflags.CF = (dest & (1 << (src - 1))) > 0;

    return res;
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_sar(src, dest, data_size);
#else
//    trace_alu("ORIGIN src %x, dest %x, size %d", src, dest, data_size);

    uint32_t mask = ((uint32_t) 0xFFFFFFFF >> (32 - data_size));

    src &= mask;
    dest &= mask;

    uint32_t sign = dest & (1 << (data_size - 1));

    uint32_t res = (dest >> src);

    // sign ext
    if (sign) {
        res |= (uint32_t) 0xFFFFFFFF << (data_size - src);
    }
    res &= mask;

    alu_set_flag_bits(res, data_size);

    // set CF
    cpu.eflags.CF = (dest & (1 << (src - 1))) > 0;

    return res;
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_sal(src, dest, data_size);
#else

//    trace_alu("ORIGIN src %x, dest %x, size %d", src, dest, data_size);

    uint32_t mask = ((uint32_t) 0xFFFFFFFF >> (32 - data_size));
    src &= mask;
    dest &= mask;

    uint32_t res = (dest << src) & mask;
    alu_set_flag_bits(res, data_size);

    // set CF
    cpu.eflags.CF = (dest & (1 << (data_size - src))) > 0;
    return res;
#endif
}
