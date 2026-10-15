#ifndef REGISTERS_H
#define REGISTERS_H
#include "config.h"

struct registers
{
    uint8_t V_registers[V_REGISTERS_NUMBER];
    uint16_t I_register;
    uint8_t delay_register;
    uint8_t sound_timer_register;
    uint16_t Program_Counter;
    uint8_t Stack_pointer;
};

#endif