#include <stdio.h>
#include "SDL3/SDL.h"
#include <stdbool.h>
#include <stdint.h>
#include "chip8.h"
#include "keyboard.h"
#include "config.h"
#include <windows.h>
#include "memory.h"

int main(int argc, char **argv)
{
    if(argc < 2){
        printf("voce precisa adicionar o diretorio da rom");
        return -1;
    }
    
    const char* room_name = argv[1]; 
    printf("%s\n",room_name);
    FILE* room = fopen(room_name,"rb");
    if(!room){
        printf("rom não encontrada");
        return -1;
    }
    fseek(room,0,SEEK_END);
    long size = ftell(room);
    fseek(room,0,SEEK_SET);
    
    char room_read[size];
    int res = fread(room_read,size,1,room);
    if(res!=1){
        printf("falha na leitura da room");
        return -1;
    }
    fclose(room);

    printf("%s\n",room_read);

    struct chip8 chip8;
    chip8_init(&chip8);
    chip8_load_rom(&chip8,room_read,size);
    const uint8_t keyboard_map[KEYBOARD_TOTAL_KEYS] = {
        SDLK_0, SDLK_1, SDLK_2, SDLK_3,
        SDLK_4, SDLK_5, SDLK_6, SDLK_7,
        SDLK_8, SDLK_9, SDLK_A, SDLK_B,
        SDLK_C, SDLK_D, SDLK_E, SDLK_F};


    //chip8_screen_clear(&chip8.scren);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_TITTLE,
        WIDTH * WINDOW_MULTIPLIER,
        HEIGHT * WINDOW_MULTIPLIER,
        0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    SDL_Event event;
    SDL_Keycode key;
    uint32_t last_time = SDL_GetTicks();
    const int instructions_per_frame = 10;

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                goto exit;
                break;
            case SDL_EVENT_KEY_DOWN:
                key = event.key.key;
                uint8_t vkey = chip8_keyboard_map(keyboard_map, key);
                if (vkey != -1)
                {
                    chip8_keyboard_down(&chip8.keyboard, vkey);
                }
                break;
            case SDL_EVENT_KEY_UP:
                key = event.key.key;
                vkey = chip8_keyboard_map(keyboard_map, key);
                if (vkey != -1)
                {
                    chip8_keyboard_up(&chip8.keyboard, vkey);
                }
                break;
            }
        }

        for (int i = 0; i < instructions_per_frame; i++) {
            uint16_t instruction = chip8_get_instruction(&chip8, chip8.chip8_registers.Program_Counter);
            chip8.chip8_registers.Program_Counter += 2;
            chip8_exec(&chip8, instruction);
            printf("%X\n",instruction);

        }

        uint32_t current_time = SDL_GetTicks();
        if (current_time - last_time >= 16) {
            if (chip8.chip8_registers.delay_register > 0) {
                chip8.chip8_registers.delay_register -= 1;
            }
            if (chip8.chip8_registers.sound_timer_register > 0) {
                Beep(9000, 50);
                chip8.chip8_registers.sound_timer_register -= 1;
            }
            last_time = current_time;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                if(chip8_screen_is_set(&chip8.scren,x,y)){
                SDL_FRect r;
                r.x = x * WINDOW_MULTIPLIER;
                r.y = y * WINDOW_MULTIPLIER;
                r.h = WINDOW_MULTIPLIER;
                r.w = WINDOW_MULTIPLIER;
                SDL_RenderFillRect(renderer, &r);
                }
            }
        }
        SDL_RenderPresent(renderer);


        if(chip8.chip8_registers.delay_register > 0){
            SDL_Delay(10);
            chip8.chip8_registers.delay_register -=1;
        }

        if(chip8.chip8_registers.sound_timer_register > 0){
            printf("sss");
            Beep(7000,100);
            chip8.chip8_registers.sound_timer_register -=1;
        }
        SDL_Delay(2);
    }

exit:
    SDL_DestroyWindow(window);
    return 0;
}
