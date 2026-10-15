#include "chip8.h"
#include <memory.h>
#include <string.h>
#include <assert.h>
#include "config.h"
#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

const char chip8_characters_defout []= {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


void chip8_load_rom(struct chip8* chip8, const char* room, size_t lenght){
    assert(lenght<(EMULATOR_MEMORY_SIZE-PROGRAM_LOAD_ADDRESS));
    memcpy(&chip8->chip8_memory.memory[PROGRAM_LOAD_ADDRESS],room,lenght);
    chip8->chip8_registers.Program_Counter = PROGRAM_LOAD_ADDRESS;
}

void chip8_exec_especial_instructions(struct chip8* chip8,uint16_t instruction){
    uint16_t nnn = instruction & 0x0FFF;
    uint16_t x = (instruction & 0x0F00) >> 8;
    uint16_t y = (instruction & 0x00F0) >> 4;
    uint16_t kk = instruction & 0x00FF;
    uint16_t n = (instruction & 0x000F);
    
    switch (instruction & 0xF000)
    {
    case 0x1000: //JP ADDR
        chip8->chip8_registers.Program_Counter = nnn; 
        break;

    case 0x2000: //CALL addr
        chip8_stack_push(chip8, chip8->chip8_registers.Program_Counter);
        chip8->chip8_registers.Program_Counter = nnn;
        break;

    case 0x3000: //SE VX, BYTE
        if(chip8->chip8_registers.V_registers[x] == kk){
            chip8->chip8_registers.Program_Counter +=2;
        }
        break;

    case 0x4000: // SNE VX, BYTE
        if(chip8->chip8_registers.V_registers[x] != kk){
            chip8->chip8_registers.Program_Counter +=2;
        }
        break;

    case 0x5000: //SE VX, BYTE
        if(chip8->chip8_registers.V_registers[x] == chip8->chip8_registers.V_registers[y]){
            chip8->chip8_registers.Program_Counter +=2;
        }
        break;

    case 0x6000: // LD VX, BYTE
        chip8->chip8_registers.V_registers[x] = kk;
        break;

    case 0x7000: // ADD VX,BYTE
        chip8->chip8_registers.V_registers[x] += kk;
        break;

    case 0x8000: // LD VX,VY
        switch (instruction & 0x000F){
        case 0x0:
            chip8->chip8_registers.V_registers[x] = chip8->chip8_registers.V_registers[y];
            break;

        case 0x1:
            chip8->chip8_registers.V_registers[x] |= chip8->chip8_registers.V_registers[y];
            break;

        case 0x2:
            chip8->chip8_registers.V_registers[x] &= chip8->chip8_registers.V_registers[y];
            break;

        case 0x3:
            chip8->chip8_registers.V_registers[x] ^= chip8->chip8_registers.V_registers[y];
            break;
        case 0x4:
        {
            uint16_t sum = chip8->chip8_registers.V_registers[x] + chip8->chip8_registers.V_registers[y];
            chip8->chip8_registers.V_registers[0xF] = (sum > 255) ? 1 : 0;
            chip8->chip8_registers.V_registers[x] = sum;
            break;
        }
        case 0x5:
            chip8->chip8_registers.V_registers[x] -= chip8->chip8_registers.V_registers[y];
            chip8->chip8_registers.V_registers[0xF] = (chip8->chip8_registers.V_registers[y] <= chip8->chip8_registers.V_registers[x]) ? 1 : 0;
            break;
        case 0x6:
            chip8->chip8_registers.V_registers[0xF] = ((chip8->chip8_registers.V_registers[x] & 0b00000001) == 0b1) ? 1 : 0;
            chip8->chip8_registers.V_registers[x] >>=1;
            break;
        case 0x7:
            chip8->chip8_registers.V_registers[x] = chip8->chip8_registers.V_registers[y] - chip8->chip8_registers.V_registers[x];
            chip8->chip8_registers.V_registers[0xF] = (chip8->chip8_registers.V_registers[y] >= chip8->chip8_registers.V_registers[x]) ? 1 : 0;
            break;
        case 0xE:
            chip8->chip8_registers.V_registers[0xF] = ((chip8->chip8_registers.V_registers[x] & 0b10000000) == 0b10000000) ? 1 : 0;
            chip8->chip8_registers.V_registers[x] <<=1;
            break;
        default:
            printf("instrução 8xyb não reconhecida");
            break;
        }
        break;
    case 0x9000:
        if(chip8->chip8_registers.V_registers[x] != chip8->chip8_registers.V_registers[y]){
            chip8->chip8_registers.Program_Counter +=2;
        }
        break;
    case 0xA000:
        chip8->chip8_registers.I_register = nnn;
        break;
    case 0xB000:
        chip8->chip8_registers.Program_Counter = nnn + chip8->chip8_registers.V_registers[0];
        break;
    case 0xC000:
    {
        uint8_t randon = rand() % 256;
        chip8->chip8_registers.V_registers[x] = kk & randon;
        break;
    }
    case 0xD000:
{
    uint8_t x_coord = chip8->chip8_registers.V_registers[x];
    uint8_t y_coord = chip8->chip8_registers.V_registers[y];
    uint16_t I = chip8->chip8_registers.I_register;
    
    chip8->chip8_registers.V_registers[0xF] = chip8_screen_draw_sprite(
        &chip8->scren, 
        x_coord, 
        y_coord, 
        &chip8->chip8_memory.memory[I], 
        n
    );
    break;
}
    case 0xE000:
        switch (instruction & 0x00FF)
        {
        case 0x9E:
            if(chip8_keyboard_is_down(&chip8->keyboard,chip8->chip8_registers.V_registers[x])){
                chip8->chip8_registers.Program_Counter += 2;
            }
            break;
        case 0xA1:
            if(chip8_keyboard_is_down(&chip8->keyboard,chip8->chip8_registers.V_registers[x]) == false){
                chip8->chip8_registers.Program_Counter += 2;
            }
            break;
        default:
            printf("instrução Exbb não reconhecida");
            break;
        }
        break;
    case 0xF000:
        switch (instruction & 0x00FF)
        {
        case 0x07:
            chip8->chip8_registers.V_registers[x] = chip8->chip8_registers.delay_register;
            break;
        case 0x0A:
        {
            bool key_pressed = false;
            for(int i=0;i<KEYBOARD_TOTAL_KEYS;i++){
                if(chip8_keyboard_is_down(&chip8->keyboard,i)){
                    chip8->chip8_registers.V_registers[x] = i;
                    key_pressed = true;
                    chip8->chip8_registers.Program_Counter +=2;
                    break;
                }
            }
            if(!key_pressed){
                chip8->chip8_registers.Program_Counter -=2;
            }
            break;
        }
        case 0x15:
            chip8->chip8_registers.delay_register = chip8->chip8_registers.V_registers[x]; 
            break;
        case 0x18:
            chip8->chip8_registers.sound_timer_register = chip8->chip8_registers.V_registers[x]; 
            break;
        case 0x1E:
            chip8->chip8_registers.I_register += chip8->chip8_registers.V_registers[x];
            break;
        case 0x29:
        {
            uint8_t vx_value = chip8->chip8_registers.V_registers[x];
            chip8->chip8_registers.I_register = 0x050 + (vx_value * 5);
            break;   
        }
        case 0x33:
        {
            uint8_t valor = chip8->chip8_registers.V_registers[x];
            chip8_memory_set(&chip8->chip8_memory, chip8->chip8_registers.I_register, valor / 100);
            chip8_memory_set(&chip8->chip8_memory, chip8->chip8_registers.I_register + 1, (valor / 10) % 10);
            chip8_memory_set(&chip8->chip8_memory, chip8->chip8_registers.I_register + 2, valor % 10);   
            break;
        }
        case 0x55:
            for(int i=0;i<=x;i++){
            uint8_t valor = chip8->chip8_registers.V_registers[i];
            chip8_memory_set(&chip8->chip8_memory, chip8->chip8_registers.I_register + i, valor);
            }
            break;
        case 0x65:
            for(int i=0;i<=x;i++){
            chip8->chip8_registers.V_registers[i] = chip8_memory_get(&chip8->chip8_memory, chip8->chip8_registers.I_register + i);
            }
            break; 
        default:
            printf("comando 0xFxbb desconhecido");
            break;
        }        
    }
}

void chip8_exec(struct chip8* chip8,uint16_t instruction){
    switch (instruction)
    {
    case 0x00E0:
        chip8_screen_clear(&chip8->scren);
        break;
    case 0x00EE:
        chip8->chip8_registers.Program_Counter = chip8_stack_pop(chip8);
        break;
    default:
        chip8_exec_especial_instructions(chip8,instruction);
        break;
    }
}


void chip8_init(struct chip8* chip8){
    memset(chip8,0,sizeof(struct chip8));
    memcpy(&chip8->chip8_memory.memory[0x050], chip8_characters_defout, sizeof(chip8_characters_defout));
    srand(time(NULL));
};
