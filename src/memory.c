#include "memory.h"
#include <stdint.h>
#include "chip8.h"

void chip8_memory_set(struct memory* mem, int index, uint8_t value){
    mem->memory[index] = value;
}




unsigned char chip8_memory_get(struct memory* mem, uint8_t index){
    return mem->memory[index];
}


uint16_t chip8_get_instruction(struct chip8* chip8, int index){
    uint8_t byte1 = chip8->chip8_memory.memory[index];
    uint8_t byte2 = chip8->chip8_memory.memory[index + 1];
    return (byte1 << 8) | byte2;
}
