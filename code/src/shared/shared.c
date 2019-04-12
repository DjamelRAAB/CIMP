#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "SHARED/shared.h"
#include <sys/types.h>
#include <unistd.h>



/**
 * Initialise une fenetre et un rendu
 * @param  Deux pointeur (1er -> *window, 2ème -> *renduerer) 
 * @param  w : la largeur, h : la hauteur 
 * @return return 0 si tout se passe bien -1 si non
 **/
int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h){
  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS )!=0){
    fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
    return -1;
  }

  *window = SDL_CreateWindow("CIMP_RMH_SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,w, h, SDL_WINDOW_RESIZABLE |SDL_WINDOW_OPENGL );

  if (window==NULL){
    fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
    return -1;
  }

  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
 
  if(renderer == NULL){
    fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
    return -1;
  }
  return 0;
}


/**
 * modifie la couleur d'un rendu 
 * @param  le renderer et la couleur de fond à lui appliqué 
 * @return return 0 si tout se passe bien -1 si non
 **/
int setWindowColor(SDL_Renderer *renderer, SDL_Color color){
  
  if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
    return -1;
  
  if(SDL_RenderClear(renderer) < 0)
    return -1;
  SDL_RenderPresent(renderer);
  return 0;  
}

