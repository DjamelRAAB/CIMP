#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef __IMAGE__PROCESSING__H__
#define __IMAGE__PROCESSING__H__

enum Processing {LEFT_ROTATION , RIGHT_ROTATION , ZOOM, FLIP_VERTICAL, FLIP_HORIZONTAL , GREY, FILLING, NEGATIVE, BRIGHTNESS, ADD_CONTRAST , SUB_CONTRAST , BLURRED, CLIPPING};

Uint8 f(Uint8 c, int n);
Uint8 fContrast(Uint8 c, double n);
Uint32 flouPixel(Uint32 * pixels, SDL_Rect *rect ,int i, int j, int n);
int imageProcessing (SDL_Renderer *renderer , SDL_Rect *rect , SDL_Color c , enum Processing act);

#endif
