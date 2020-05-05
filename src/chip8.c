#include "chip8.h"

chip8_t *init_chip8()
{
    chip8_t *chip8 = malloc(sizeof(chip8_t));
    memset(chip8, 0, sizeof(*chip8));

    return chip8;
}

void load_rom(chip8_t *c8)
{
    FILE *f = fopen("./roms/kaleidoscope.ch8", "rb");
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);
    fread(c8->cpu.mem + 0x200, 1, size, f);
    fclose(f);
}

void fetch_instruction(uint16_t *opcode, chip8_t *c8)
{
    uint8_t msb = c8->cpu.mem[c8->cpu.pc];
    uint8_t lsb = c8->cpu.mem[c8->cpu.pc + 1];
    *opcode = (msb << 8) | lsb;
    c8->cpu.pc += 2;
}

void run(chip8_t *c8)
{
    uint16_t opcode = 0;

    while (1)
    {
        fetch_instruction(&opcode, c8);
    }
}