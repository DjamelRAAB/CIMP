#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "shared.h"

/**
 * Créée un Graphe
 * @param  Deux pointeur (1er -> *window, 2ème -> *renduerer) 
 * @param  w : la largeur, h : la hauteur 
 * @return return 0 si tout se passe bien -1 si non
 **/
int init(dataWindows **windows, int w, int h){
  (*windows)->fenetre = SDL_CreateWindow("image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_BORDERLESS);
  if( (*windows)->fenetre == NULL ){
    fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
    return 0;
  }
  (*windows)->renderer = SDL_CreateRenderer((*windows)->fenetre, -1, SDL_RENDERER_ACCELERATED);
  if( (*windows)->renderer == NULL ){
    SDL_DestroyWindow((*windows)->fenetre);
    fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
    return 0;
  }

  return 1;
}


/**
 * Créée un Graphe
 * @param  le renderer et la couleur de fond à lui appliqué 
 * @return return 0 si tout se passe bien -1 si non
 **/
int setWindowColor(SDL_Renderer *renderer, SDL_Color color){
  
  if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
    return -1;
  
  if(SDL_RenderClear(renderer) < 0)
    return -1;
  
  return 0;  
  
}
