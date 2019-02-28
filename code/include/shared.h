#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __GRAPH__H__
#define __GRAPH__H__

	/* Définition d'une fonction qui modifie la coleur du renderer */

int setWindowColor(SDL_Renderer *renderer, SDL_Color color);

	/* Définition d'une fonction qui initialise un fenétre et un renderer */

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);

	/* Définition d'une fonction qui charge une image au format BMP */


SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);

#endif
