#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef __LOAD__IMAGE__H__
#define __LOAD__IMAGE__H__


SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);

#endif
