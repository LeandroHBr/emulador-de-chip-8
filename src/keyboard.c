#include "keyboard.h"
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

static void keyboard_key_avaliable(uint8_t key)
{
    assert(key >= 0 && key < KEYBOARD_TOTAL_KEYS);
}

int chip8_keyboard_map(const uint8_t *map, uint8_t key)
{
    for (int i = 0; i < KEYBOARD_TOTAL_KEYS; i++)
    {
        if (map[i] == key)
        {
            return i;
        }
    }
    return -1;
}

void chip8_keyboard_down(struct keyboard *keyboard, uint8_t key)
{
    keyboard->keyboard_keys[key] = true;
}

void chip8_keyboard_up(struct keyboard *keyboard, uint8_t key)
{
    keyboard->keyboard_keys[key] = false;
}

bool chip8_keyboard_is_down(struct keyboard *keyboard, uint8_t key)
{
    return keyboard->keyboard_keys[key];
}
