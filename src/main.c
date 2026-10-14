#include <stdio.h>
#include "SDL3/SDL.h"
#include "chip8.h"

int main(int args, char **argv)
{
    struct chip8 chip8;
    chip8.chip8_registers.Stack_pointer = 0;
    
    chip8_stack_push(&chip8, 0xbb);
    chip8_stack_push(&chip8, 0xaa);

    printf("%x\n",chip8_stack_pop(&chip8));
    printf("%x\n",chip8_stack_pop(&chip8));




    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_TITTLE,
        WIDTH,
        HEIGHT,
        0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    SDL_Event event;

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                goto exit;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_FRect r;
        r.x = 50;
        r.y = 50;
        r.h = 50;
        r.w = 50;
        SDL_RenderFillRect(renderer, &r);
        SDL_RenderPresent(renderer);
    }

exit:
    SDL_DestroyWindow(window);
    return 0;
}
