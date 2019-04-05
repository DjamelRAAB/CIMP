#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "select.h"

/**
 * La fonction select fait une selection sur un rendu 
 * @param rect : le rectangle à sélectionner
 * @param rederer : la composante rendu sur la quelle faire la selection
 * @return pixels : une matrice de pixels qui représente le rectangle sélectionner
 **/
int getPixels(SDL_Renderer *renderer , SDL_Rect *rect , Uint32 * pixels){
  if (SDL_RenderReadPixels(renderer, rect, SDL_PIXELFORMAT_RGBA8888, pixels, sizeof(Uint32)* rect->w)){
    fprintf(stderr, "Erreur  in select :  SDL_RenderReadPixels: %s", SDL_GetError());
    return -1;
  }
  return 0 ;
}

/**
 * La fonction selection un rectongle a la souris 
 * @return rect : le rectangle sélectionner
 **/
SDL_Rect selectRect (){
  SDL_Event e;
  SDL_Point src ,dst;
  SDL_Rect rect;
  int b=0 ;
  while(b == 0){
    if (SDL_PollEvent(&e)) {
      switch(e.type){
        case SDL_MOUSEBUTTONDOWN :
          if(e.button.button==SDL_BUTTON_LEFT) {
          src.x = e.button.x, 
          src.y = e.button.y;
          }      
        break;
        case SDL_MOUSEBUTTONUP :
        if(e.button.button==SDL_BUTTON_LEFT) {
          dst.x = e.button.x, 
          dst.y = e.button.y;    
        }
        b=1;
        break;
      }
    }
  }
  if(src.x<dst.x){
    rect.x=src.x;
    rect.y=src.y;
  }else{
    rect.x=dst.x;
    rect.y=dst.y;
  }
  rect.h = abs(src.y - dst.y);
  rect.w = abs(src.x - dst.x);
  return rect ; 
}