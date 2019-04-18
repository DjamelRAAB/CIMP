#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "windows.h"


windows *initWindows(const char path[]){
    SDL_Color defaultColor = {255,255,0,0}; 
    SDL_Window *f = NULL;

    windows *w = malloc(sizeof(windows));
    sprintf(w->path, "%s", path);
    w->color = defaultColor;
    w->reduite = 0;
    w->id = 0;
    w->fenetre = f;

    return w; 
}