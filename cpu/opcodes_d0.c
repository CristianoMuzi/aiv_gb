#include <aiv_gb.h>

//RET NC
static int aiv_gb_opcode_d0(aiv_gameboy *gb)
{
    if (aiv_gb_get_flag(gb, CARRY)==CARRY)
    {
        return 8;
    }

    gb->pc = aiv_gb_memory_read16(gb, gb->sp + 1);
    gb->sp += 2;

    return 20;
}

//POP DE
static int aiv_gb_opcode_d1(aiv_gameboy *gb)
{
    gb->de = aiv_gb_memory_read16(gb, gb->sp + 1);
    gb->sp += 2;

    return 12;
}

//JP NC,a16
static int aiv_gb_opcode_d2(aiv_gameboy *gb)
{
    //check if C == 1
    if (aiv_gb_get_flag(gb, CARRY)==CARRY)
    {
        gb->pc += 2;
        return 12;
    }

    gb->pc = aiv_gb_memory_read16(gb, gb->pc);
    return 16;
}

//CALL NC,a16
static int aiv_gb_opcode_d4(aiv_gameboy *gb)
{
    //check if C == 1
    if (aiv_gb_get_flag(gb, CARRY)==CARRY)
    {
        gb->pc += 2;
        return 12;
    }

    aiv_gb_memory_write8(gb, gb->sp, gb->pc + 2);
    gb->sp -= 1;

    gb->pc = aiv_gb_memory_read16(gb, gb->pc);

    return 24;
}

//PUSH DE
static int aiv_gb_opcode_d5(aiv_gameboy *gb)
{
    aiv_gb_memory_write16(gb, gb->sp - 1, gb->de);
    gb->sp -= 2;

    return 16;
}

//SUB d8
static int aiv_gb_opcode_d6(aiv_gameboy *gb)
{
    u8_t n1 = gb->a;
    u8_t n2 = aiv_gb_memory_read8(gb, gb->pc);
    gb->pc++;

    gb->a = n1 - n2;

    aiv_gb_set_flag(gb, ZERO, gb->a == 0);
    aiv_gb_set_flag(gb, NEG, 1);
    aiv_gb_set_flag(gb, HALF, (((n1 & 0x0f) - n2) & 0xf0));
    aiv_gb_set_flag(gb, CARRY, n1 >= n2);

    return 8;
}

//RST 10H

//RET C
static int aiv_gb_opcode_d8(aiv_gameboy *gb)
{
    //check if C == 0
    if (!aiv_gb_get_flag(gb, CARRY))
    {
        return 8;
    }

    gb->pc = aiv_gb_memory_read16(gb, gb->sp + 1);
    gb->sp += 2;

    return 20;
}

//RETI

//JP C,a16

//CALL C,a16

//SBC A,d8

//RST 18H

void aiv_gb_register_opcodes_d0(aiv_gameboy *gb)
{
    gb->opcodes[0xd0] = aiv_gb_opcode_d0;
    gb->opcodes[0xd1] = aiv_gb_opcode_d1;
    gb->opcodes[0xd2] = aiv_gb_opcode_d2;
    gb->opcodes[0xd4] = aiv_gb_opcode_d4;
    gb->opcodes[0xd5] = aiv_gb_opcode_d5;
    gb->opcodes[0xd6] = aiv_gb_opcode_d6;
    gb->opcodes[0xd8] = aiv_gb_opcode_d8;
}
