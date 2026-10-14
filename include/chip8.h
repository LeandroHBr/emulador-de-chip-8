#ifndef CHIP8_H
#define CHIP8_H
#include <stdint.h>
#include "config.h"
#include "memory.h"
#include "registers.h"
#include "chip8stack.h"

struct chip8
{
    struct memory chip8_memory;
    struct registers chip8_registers;
    struct chip8stack stack;
};


#endif