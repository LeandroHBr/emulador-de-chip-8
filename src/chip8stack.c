
#include "chip8stack.h"
#include "chip8.h"
#include <assert.h>

static void chip8_stack_in_bound(struct chip8* chip8)
{
    assert(chip8->chip8_registers.Stack_pointer < STACK_REGISTERS_NUMBER);
};

void chip8_stack_push(struct chip8* chip8, uint16_t value){
    chip8_stack_in_bound(chip8);
    chip8->stack.stack[chip8->chip8_registers.Stack_pointer] = value; 
    chip8->chip8_registers.Stack_pointer += 1;
}

uint16_t chip8_stack_pop(struct chip8* chip8){
    chip8->chip8_registers.Stack_pointer -=1;
    uint16_t value = chip8->stack.stack[chip8->chip8_registers.Stack_pointer];
    //chip8->stack.stack[chip8->chip8_registers.Stack_pointer] = 0;
    return value;
}