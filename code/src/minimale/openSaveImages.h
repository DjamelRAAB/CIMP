#include  <SDL2/SDL.h>
#include "../windows/windows.h"

#ifndef __OPENSAVEIMAGES__H__
#define __OPENSAVEIMAGES__H__

int openImages(char *path[], int nbImages, windows **tetelisteWindows, int *posCurrent);
/* Définition d'une fonction qui charge une image au format BMP */
SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);

void closeImage(dataWindows **w);
#endif