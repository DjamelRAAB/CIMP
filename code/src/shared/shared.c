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
int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h){
  if(SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_MINIMIZED, window, renderer) !=0 ){   
    fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
    return -1;
  }
  return 0;
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
