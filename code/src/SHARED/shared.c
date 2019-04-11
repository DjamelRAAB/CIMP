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
  if(SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN, window, renderer) !=0 ){   
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



/**
 * Créée un Graphe
 * @param  path : le chemin absolut ou relatif vers l'image
 * @param  la composante renderer sur la quelle on veux chargé l'image
 * @return Une SDL_Texture modifiable qui contient l'image qu'on veux chargé  
 **/
SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer){

  SDL_Surface *tmp = NULL; 
  SDL_Texture *texture = NULL;
  SDL_Texture *texture_enable = NULL;

  tmp = SDL_LoadBMP(path);

  if(tmp == NULL){
    fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
    return NULL;
  }
    
  texture = SDL_CreateTextureFromSurface(renderer, tmp);

  SDL_FreeSurface(tmp);
    
  if(texture == NULL){
    fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
    return NULL;
  }

  SDL_Rect rect = {0, 0, 0, 0};
  SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

  texture_enable = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);

  if(texture_enable == NULL){
    fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
    return NULL;
  }
  
  SDL_SetRenderTarget(renderer, texture_enable);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_SetRenderTarget(renderer, NULL);

  return texture_enable;

}