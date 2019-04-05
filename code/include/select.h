#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef __SELECT__H__
#define __SELECT__H__


int getPixels(SDL_Renderer *renderer , SDL_Rect *rect, Uint32 * pixels );
SDL_Rect selectRect ();

#endif
