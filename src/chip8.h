#ifndef CHIP8_H
#define CHIP8_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define SCREEN_W 64
#define SCREEN_H 32

static const uint8_t font[0x10][5] = {
    {0xf0, 0x90, 0x90, 0x90, 0xf0}, // 0
    {0x20, 0x60, 0x20, 0x20, 0x70}, // 1
    {0xf0, 0x10, 0xf0, 0x80, 0xf0}, // 2
    {0xf0, 0x10, 0xf0, 0x10, 0xf0}, // 3
    {0x90, 0x90, 0xf0, 0x10, 0x10}, // 4
    {0xf0, 0x80, 0xf0, 0x10, 0xf0}, // 5
    {0xf0, 0x80, 0xf0, 0x90, 0xf0}, // 6
    {0xf0, 0x10, 0x20, 0x40, 0x40}, // 7
    {0xf0, 0x90, 0xf0, 0x90, 0xf0}, // 8
    {0xf0, 0x90, 0xf0, 0x10, 0xf0}, // 9
    {0xf0, 0x90, 0xf0, 0x90, 0x90}, // a
    {0xe0, 0x90, 0xe0, 0x90, 0xe0}, // b
    {0xf0, 0x80, 0x80, 0x80, 0xf0}, // c
    {0xe0, 0x90, 0x90, 0x90, 0xe0}, // d
    {0xf0, 0x80, 0xf0, 0x80, 0xf0}, // e
    {0xf0, 0x80, 0xf0, 0x80, 0x80}  // f
};

typedef struct cpu_t
{
    uint8_t mem[0xfff];
    uint16_t stack[0xf];
    uint8_t reg[0xf];
    uint16_t i;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t sp;
    uint16_t pc;
} cpu_t;

typedef struct chip8_t
{
    cpu_t cpu;
    uint8_t framebuf[SCREEN_W][SCREEN_H];
} chip8_t;

extern chip8_t *init_chip8();
extern void load_rom(chip8_t *);
extern void run(chip8_t *);

#endif
