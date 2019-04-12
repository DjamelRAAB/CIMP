#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef __LOAD__SAVE__H__
#define __LOAD__SAVE__H__


SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer , int boolAlpha);
int saveImage(SDL_Window* window, SDL_Renderer *renderer, char *name, char *format );


#endif
