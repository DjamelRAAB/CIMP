//gcc -Wall -Wextra main.c $(sdl2-config --cflags --libs)

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "load_save.h"
#include "shared.h"
#include "select.h"
#include "image_processing.h"



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

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  
  texture_image = loadImage("./asserts/katoche.jpg", renderer , 0);
  
  if(texture_image == NULL){
    fprintf(stderr, "Erreur loadImage : %s", SDL_GetError());
    goto Quit;
    
  }
  
  SDL_RenderCopy(renderer, texture_image, NULL, NULL);


  SDL_RenderPresent(renderer);
  
  statut = EXIT_SUCCESS;

  SDL_Delay(1000);

  SDL_Rect rect = selectRect (renderer);
  SDL_Color c = {255, 255 ,255 ,155};
  /*enum Processing act = ZOOM ;
  imageProcessing (renderer , &rect , c , act);
  */

  SDL_Texture * t = couper(renderer , &rect , c);

  SDL_Rect rect2 = selectRect (renderer);

  coller(renderer , &rect2 ,t );

  SDL_Rect rect3 = selectRect (renderer);

  t = copier(renderer , &rect3 );

  SDL_Rect rect4 = selectRect (renderer);

  coller(renderer , &rect4 ,t );
  SDL_Delay(1000);
  

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
