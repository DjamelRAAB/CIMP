#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "windows.h"

/**
 * 
**/

windows *initWindows(const char path[]){
    SDL_Color defaultColor = {255,255,0,0}; 
    SDL_Window *f = NULL;
    SDL_Renderer *r = NULL;

    windows *w = malloc(sizeof(windows));
    sprintf(w->path, "%s", path);
    w->color = defaultColor;
    w->reduite = 0;
    w->fermee = 0;
    w->fenetre = f;
    w->renderer = r;
    w->id = 0;

    return w; 
}

