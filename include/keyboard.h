#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "config.h"
#include <stdbool.h>
#include <stdint.h>

struct keyboard
{
    bool keyboard_keys[KEYBOARD_TOTAL_KEYS];
};


int chip8_keyboard_map(const uint8_t* map, uint8_t key);
void chip8_keyboard_down(struct keyboard* keyboard,uint8_t key);
void chip8_keyboard_up(struct keyboard* keyboard,uint8_t key);
bool chip8_keyboard_is_down(struct keyboard* keyboard,uint8_t key);
#endif