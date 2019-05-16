#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "select.h"
#include "image_processing.h"

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
 * @param renderer : le rendu sure le quel travailler.
 * @return rect : le rectangle sélectionner.
 **/
SDL_Rect selectRect (SDL_Renderer *renderer){
  SDL_Event *e =malloc (sizeof(SDL_Event));
  int b=0 ,down = 0 ,pitch;
  SDL_Point src ,dst;
  SDL_Rect rect;
  SDL_Rect tmp ;
  Uint32 *pixels; 
  SDL_Texture *texture ; 
  SDL_RenderGetViewport(renderer, &tmp);//on récupère la zone de travaille dans rect
  pitch = sizeof(Uint32) * tmp.w ;
  pixels = malloc(pitch * tmp.h);
  if(pixels == NULL){
    return tmp;
  }
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, tmp.w, tmp.h );
  if(texture == NULL){
    free(pixels);
    return tmp;
  }
  getPixels(renderer , &tmp ,pixels);
  SDL_UpdateTexture(texture, NULL, pixels, sizeof(Uint32) * tmp.w);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  while(b == 0){
    if(SDL_PollEvent(e)){
    switch(e->type){
        case SDL_MOUSEBUTTONDOWN :
          if(e->button.button==SDL_BUTTON_LEFT) {
          src.x = e->button.x, 
          src.y = e->button.y;
          }      
          down=1;
          break;
        case SDL_MOUSEMOTION :
          if(down==1){
            dst.x = e->button.x, 
            dst.y = e->button.y;    
            if(src.x<dst.x)
              rect.x=src.x;
            else
              rect.x=dst.x;
            if(src.y<dst.y)
              rect.y=src.y;
            else
              rect.y=dst.y;
          rect.h = abs(src.y - dst.y);
          rect.w = abs(src.x - dst.x);
          SDL_RenderCopy(renderer, texture, NULL, NULL);
          SDL_RenderDrawRect(renderer, &rect); 
          //SDL_RenderCopy(renderer, texture, &rect, NULL);          
          SDL_RenderPresent(renderer);
          }
          break;
        case SDL_MOUSEBUTTONUP :
          down =0;
          if(e->button.button==SDL_BUTTON_LEFT) {
            dst.x = e->button.x, 
            dst.y = e->button.y;  
            rect.h = abs(src.y - dst.y);
          rect.w = abs(src.x - dst.x);
          }
        b=1;
        break;
        case SDL_QUIT : 
          goto Quit;
          break;
      }
    }
  }
  Quit :
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_DestroyTexture(texture);
  free(pixels);
  return rect ; 
}

/**
 * La fonction copier renvoi une texture qui contient une copie du rectangle sélectionner 
 * @param renderer : le rendu sure le quel travailler.
 * @param rect : le rectangle à sélectionner.
 * @return  un pointeur vers une texture->
 **/
SDL_Texture * copier(SDL_Renderer *renderer , SDL_Rect *rect ){
  int pitch ;
  Uint32 *pixels; 
  SDL_Texture *texture ; 
  pitch = sizeof(Uint32) * rect->w ;
  pixels = malloc(pitch * rect->h);
  if(pixels == NULL){
    return NULL;
  }
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, rect->w, rect->h );
  if(texture == NULL){
    free(pixels);
    return NULL;
  }
  getPixels(renderer , rect , pixels );
  SDL_UpdateTexture(texture, NULL, pixels, sizeof(Uint32) * rect->w);
  coller(renderer,rect,texture);
  return texture;
}

/**
 * La fonction couper renvoi une texture qui contient une copie du rectangle sélectionner
 * et remplie le rectangle par un couleur passer en paramétre  
 * @param renderer : le rendu sure le quel travailler.
 * @param rect : le rectangle à sélectionner.
 * @param color : la couleur avec la quel la parti sélectionner sera remplie
 * @return  un pointeur vers une texture->
 **/
SDL_Texture * couper(SDL_Renderer *renderer , SDL_Rect *rect , SDL_Color color){
  SDL_Texture *texture = copier(renderer , rect );
  enum Processing act = FILLING ;
  imageProcessing (&renderer , rect , color , act);
  return texture;
}

/**
 * La fonction coller : coller une texture sur un rendu 
 * @param renderer : le rendu sure le quel travailler.
 * @param texture  : la texture a coller.
 * @return void.
 **/
void coller(SDL_Renderer *renderer , SDL_Rect *rect, SDL_Texture *texture){  
  SDL_RenderCopy(renderer, texture, NULL, rect);
  SDL_RenderPresent(renderer);
  SDL_Delay(50);
}