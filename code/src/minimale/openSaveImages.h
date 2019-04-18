#include  <SDL2/SDL.h>
#include "../windows/windows.h"

#ifndef __OPENSAVEIMAGES__H__
#define __OPENSAVEIMAGES__H__

windows *openImages(const char path[]);
/* Définition d'une fonction qui charge une image au format BMP */
SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);

void closeImage(windows *w);
#endif