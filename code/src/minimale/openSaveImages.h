#include  <SDL2/SDL.h>
#include "../windows/windows.h"

#ifndef __OPENSAVEIMAGES__H__
#define __OPENSAVEIMAGES__H__

windows *openImages(const char path[]);
SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer, int boolAlpha);

#endif