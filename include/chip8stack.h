#ifndef CHIP8STACK_H
#define CHIP8STACK_H
#include "config.h"
#include <stdint.h>

struct chip8;

struct chip8stack
{
    uint16_t stack[STACK_REGISTERS_NUMBER];
};

void chip8_stack_push(struct chip8* chip8, uint16_t value);
uint16_t chip8_stack_pop(struct chip8 *chip8);
#endif