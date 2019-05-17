#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "../windows/windows.h"

#ifndef __GRAPH__H__
#define __GRAPH__H__

	/* Définition d'une fonction qui modifie la coleur du renderer */
int setWindowColor(SDL_Renderer *renderer, SDL_Color color);

	/* Définition d'une fonction qui initialise un fenétre et un renderer */
int init(dataWindows **windows, int w, int h);

#endif
