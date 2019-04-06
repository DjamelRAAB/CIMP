#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef __SELECT__H__
#define __SELECT__H__


int getPixels(SDL_Renderer *renderer , SDL_Rect *rect, Uint32 * pixels );
SDL_Rect selectRect (SDL_Renderer *renderer );
void coller(SDL_Renderer *renderer , SDL_Rect *rect, SDL_Texture *s);
SDL_Texture * couper(SDL_Renderer *renderer , SDL_Rect *rect , SDL_Color c);
SDL_Texture * copier(SDL_Renderer *renderer , SDL_Rect *rect );
void updat (SDL_Renderer *renderer );

#endif
