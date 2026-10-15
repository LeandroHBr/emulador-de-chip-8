#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#include "config.h"

struct chip8;

struct memory
{
    uint8_t memory[EMULATOR_MEMORY_SIZE];    
};

void chip8_memory_set(struct memory* mem, int index, uint8_t value);
unsigned char chip8_memory_get(struct memory* mem, uint8_t index);
uint16_t chip8_get_instruction(struct chip8* chip8, int index);

#endif
