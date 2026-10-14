#include "memory.h"

void chip8_memory_set(struct memory* mem, int index, uint8_t value){
    mem->memory[index] = value;
}




unsigned char chip8_memory_get(struct memory* mem, uint8_t index){
    return mem->memory[index];
}