#ifndef CHIP8_H
#define CHIP8_H
#include <stdint.h>
#include "config.h"
#include "memory.h"
#include "registers.h"
#include "chip8stack.h"
#include "keyboard.h"
#include "chip8screen.h"
#include <string.h>

struct chip8
{
    struct memory chip8_memory;
    struct registers chip8_registers;
    struct chip8stack stack;
    struct chip8screen scren;
    struct keyboard keyboard;
};

void chip8_init(struct chip8* chip8);
void chip8_load_rom(struct chip8* chip8, const char* room, size_t lenght);
void chip8_exec(struct chip8* chip8,uint16_t instruction);


#endif