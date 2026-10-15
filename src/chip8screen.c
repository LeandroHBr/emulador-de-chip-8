#include "chip8screen.h"
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>


static void chip8_screen_check_limits(int x,int y){
    assert(x>=0 && x < WIDTH && y>=0 && y< HEIGHT);    
}

void chip8_screen_set(struct chip8screen* screen, int x, int y){
    chip8_screen_check_limits(x,y);
    screen->screen_pixels[x][y] = true;
}


void chip8_screen_clear(struct chip8screen* screen){
    memset(screen->screen_pixels,0,sizeof(screen->screen_pixels));
}



bool chip8_screen_is_set(struct chip8screen* screen, int x, int y){
    chip8_screen_check_limits(x,y);
    return screen->screen_pixels[x][y];
}

bool chip8_screen_draw_sprite(struct chip8screen* screen,int x, int y,const char* sprite,int n_bytes){
    bool pixel_changed = false;

    for(int row=0; row<n_bytes;row++){
        uint8_t sprite_anilized = sprite[row];
        for(int col=0;col<8;col++){
            if((sprite_anilized & (0b10000000 >> col)) == 0){
                continue;
            }
            if(screen->screen_pixels[(x + col)%WIDTH][(y + row)%HEIGHT]){
                pixel_changed = true;
            }
            screen->screen_pixels[(x + col)%WIDTH][(y + row)%HEIGHT] ^= true;
        }
    }

    return pixel_changed;
}

