#ifndef MEMORY_H
#define MEMORY_H

struct memory
{
    uint8_t memory[EMULATOR_MEMORY_SIZE];    
};

void chip8_memory_set(struct memory* mem, int index, uint8_t value);
unsigned char chip8_memory_get(struct memory* mem, uint8_t index);


#endif
