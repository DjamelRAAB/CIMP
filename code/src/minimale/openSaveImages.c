#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>
#include "../windows/windows.h"
#include "../shared/shared.h"
#include "openSaveImages.h"
#include <SDL2/SDL_image.h>

/**
 * 
 * 
**/

windows *openImages(const char path[]){
    SDL_Window *p = NULL;
    windows *w = malloc(sizeof(windows));
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture_image = NULL;

    if(init(&p, &renderer, W, H) != 0){
        fprintf(stderr, "Erreur SDL_CreateWindow or SDL_CreateRenderer : %s", SDL_GetError());
        return NULL;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    texture_image = loadImage(path, renderer, 0);
    if(texture_image == NULL){
        fprintf(stderr, "Erreur loadImage : %s", SDL_GetError());
        return NULL;      
    }
    SDL_RenderCopy(renderer, texture_image, NULL, NULL);
    SDL_RenderPresent(renderer); 

    SDL_Delay(1500);

    w->fenetre = p;
    strcpy(w->path, path);

    return w;
}

/**
 * Charge une image sous format JPG PNG
 * @param  path : le chemin absolut ou relatif vers l'image
 * @param  la composante renderer sur la quelle on veux chargé l'image
 * @return Une SDL_Texture modifiable qui contient l'image qu'on veux chargé  
 **/
SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer, int boolAlpha){
  
  SDL_Surface *tmp ;
  SDL_Texture *texture;
  SDL_Texture *texture_enable;

  /*
   * possibilité de restreindre le format d'image
   * Initialiser le support pour le chargement uniquement des images PNG et JPG
   * IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
   */
  
  // IMG_load(path) suporte tout les format d'image
  tmp = IMG_Load(path);
  

  if (boolAlpha == 0 ){
    SDL_SetSurfaceBlendMode(tmp , SDL_BLENDMODE_BLEND);
  }

  
  
  texture = SDL_CreateTextureFromSurface(renderer, tmp);

  SDL_FreeSurface(tmp);

  if(texture == NULL){
    fprintf(stderr, "zeb zeb zeb \n");    
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
  /* 
   * On va dessiner sur la texture_enable l'image chargé au 
   * préalable dans texture pour avoire la possiblité de 
   * dessiner sur cette derniere
   */
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_DestroyTexture(texture);
  SDL_SetRenderTarget(renderer, NULL);
  
  return texture_enable;

}