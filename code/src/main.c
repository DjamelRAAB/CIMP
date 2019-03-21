//gcc -Wall -Wextra main.c $(sdl2-config --cflags --libs)

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "shared.h"
#include "save_image.h"



/**
 *@const w La largeur de la fenétre 
 *@const w La hauteur de la fenétre 
 **/
#define w 640
#define h 480



int main(void){

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Texture *texture_image = NULL;

  int statut = EXIT_FAILURE;
  
  
 
  if(init(&window, &renderer, w, h) != 0){
    fprintf(stderr, "Erreur SDL_CreateWindow or SDL_CreateRenderer : %s", SDL_GetError());
    goto Quit;
  }

  
  texture_image = loadImage("./asserts/katoche.jpg", renderer);
  
  if(texture_image == NULL){
    fprintf(stderr, "Erreur loadImage : %s", SDL_GetError());
    goto Quit;
    
  }
  
  SDL_RenderCopy(renderer, texture_image, NULL, NULL);


  SDL_RenderPresent(renderer);
  
  statut = EXIT_SUCCESS;
  
  SDL_Delay(10000);

  if(saveImage(window,renderer, "motoDa","png")!=0){
    fprintf(stderr, "Erreur loadImage : %s", SDL_GetError());
    goto Quit;
  }

 Quit :
  
  if(texture_image != NULL)
    SDL_DestroyTexture(texture_image);
  
  if(renderer !=NULL )
    SDL_DestroyRenderer(renderer);
  
  if(window !=NULL)
    SDL_DestroyWindow(window);
  IMG_Quit();  
  SDL_Quit();
  return statut;
}
