#include <time.h>
#include "instructions.h"

void drw(uint8_t x, uint8_t y, uint8_t n, chip8_t *c8)
{
    c8->cpu.reg[0xf] = 0;
    uint8_t row;
    uint8_t col;
    for (row = 0; row < n; row++)
    {
        uint8_t pixels = c8->cpu.mem[c8->cpu.i + row];

        for (col = 0; col < 8; col++)
        {
            uint8_t real_x = (x + col) % SCREEN_W;
            uint8_t real_y = (y + row) % SCREEN_H;
            uint8_t pixel = (pixels >> (7 - col)) & 0b1;
            if (c8->framebuf[real_x][real_y] == 1 && pixel == 0)
            {
                c8->cpu.reg[0xf] = 1;
            }
            c8->framebuf[real_x][real_y] ^= pixel;
        }
    }
}

void decode_instruction(uint16_t opcode, chip8_t *c8)
{
    uint8_t msb = (opcode >> 12) & 0xf;
    uint8_t lsb = opcode & 0xf;
    uint8_t x = (opcode >> 8) & 0xf;
    uint8_t y = (opcode >> 4) & 0xf;
    uint16_t kk = opcode & 0xff;
    uint16_t nnn = opcode & 0xfff;
    switch (msb)
    {
    case 0x0:
        switch (kk)
        {
        case 0xe0:
            memset(c8->framebuf, 0, sizeof(c8->framebuf));
            break;
        case 0xee:
            c8->cpu.pc = c8->cpu.stack[c8->cpu.sp];
            c8->cpu.sp -= 1;
            break;
        default:
            break;
        }
        break;
    case 0x1:
        c8->cpu.pc = nnn;
        break;
    case 0x2:
        c8->cpu.sp += 1;
        c8->cpu.stack[c8->cpu.sp] = c8->cpu.pc;
        c8->cpu.pc = nnn;
        break;
    case 0x3:
        if (c8->cpu.reg[x] == kk)
        {
            c8->cpu.pc += 2;
        }
        break;
    case 0x4:
        if (c8->cpu.reg[x] != kk)
        {
            c8->cpu.pc += 2;
        }
        break;
    case 0x5:
        if (c8->cpu.reg[x] != c8->cpu.reg[y])
        {
            c8->cpu.pc += 2;
        }
        break;
    case 0x6:
        c8->cpu.reg[x] = kk;
        break;
    case 0x7:
        c8->cpu.reg[x] += kk;
        break;
    case 0x8:
        switch (lsb)
        {
        case 0x0:
            c8->cpu.reg[x] = c8->cpu.reg[y];
            break;
        case 0x1:
            c8->cpu.reg[x] |= c8->cpu.reg[y];
            break;
        case 0x2:
            c8->cpu.reg[x] &= c8->cpu.reg[y];
            break;
        case 0x3:
            c8->cpu.reg[x] ^= c8->cpu.reg[y];
            break;
        case 0x4:
            c8->cpu.reg[x] += c8->cpu.reg[y];
            c8->cpu.reg[0xf] = (c8->cpu.reg[x] + c8->cpu.reg[y]) > 0xff ? 1 : 0;
            break;
        case 0x5:
            c8->cpu.reg[x] -= c8->cpu.reg[y];
            c8->cpu.reg[0xf] = c8->cpu.reg[x] > c8->cpu.reg[y] ? 1 : 0;
            break;
        case 0x6:
            c8->cpu.reg[x] >>= 1;
            c8->cpu.reg[0xf] = c8->cpu.reg[x] & 1 ? 1 : 0;
            break;
        case 0x7:
            c8->cpu.reg[x] = c8->cpu.reg[y] - c8->cpu.reg[x];
            c8->cpu.reg[0xf] = c8->cpu.reg[y] > c8->cpu.reg[x] ? 1 : 0;
            break;
        case 0xe:
            c8->cpu.reg[x] <<= 1;
            c8->cpu.reg[0xf] = c8->cpu.reg[x] & 1 ? 1 : 0;
            break;
        default:
            break;
        }
        break;
    case 0x9:
        if (c8->cpu.reg[x] != c8->cpu.reg[y])
        {
            c8->cpu.pc += 2;
        }
        break;
    case 0xa:
        c8->cpu.i = nnn;
        break;
    case 0xb:
        c8->cpu.pc = c8->cpu.reg[0x0] + nnn;
        break;
    case 0xc:
        c8->cpu.reg[x] = (rand() % 256) & kk;
        break;
    case 0xd:
        drw(x, y, lsb, c8);
        break;
    case 0xe:
        switch (kk)
        {
        case 0x9e:
            break;
        case 0xa1:
            break;
        default:
            break;
        }
    case 0xf:
        switch (kk)
        {
        case 0x07:
            c8->cpu.reg[x] = c8->cpu.delay_timer;
            break;
        case 0x0a:
            break;
        case 0x15:
            c8->cpu.delay_timer = c8->cpu.reg[x];
            break;
        case 0x18:
            c8->cpu.sound_timer = c8->cpu.reg[x];
            break;
        case 0x1e:
            c8->cpu.i += c8->cpu.reg[x];
            break;
        case 0x29:
            c8->cpu.i = c8->cpu.reg[x] * 5;
            break;
        case 0x33:
            c8->cpu.mem[c8->cpu.i + 2] = c8->cpu.reg[x] % 10;
            if (c8->cpu.reg[x] >= 10)
            {
                c8->cpu.mem[c8->cpu.i + 1] = (c8->cpu.reg[x] / 10) % 10;
            }
            if (c8->cpu.reg[x] >= 100)
            {
                c8->cpu.mem[c8->cpu.i] = c8->cpu.reg[x] / 100;
            }
            break;
        case 0x55:
            for (uint8_t i = 0; i < x; i++)
            {
                c8->cpu.mem[c8->cpu.i + i] = c8->cpu.reg[i];
            }
            break;
        case 0x65:
            for (uint8_t i = 0; i < x; i++)
            {
                c8->cpu.reg[i] = c8->cpu.mem[c8->cpu.i + i];
            }
            break;
        default:
            break;
        }
    default:
        break;
    }
}