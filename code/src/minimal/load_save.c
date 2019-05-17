#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "MINIMAL/load_save.h"
#include "MINIMAL/select.h"


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



/**
 * sauvgarder une image sous format JPG PNG bmp
 * @param window : la fenetre a sauvgarder
 * @param rederer : la composante rendu du quelle on va recupérer l'image
 * @return name : le nom qu'on veux donner au fichier
 * @return format : l'extension du fichier  
 **/

int saveImage(SDL_Window* window, SDL_Renderer *renderer, char *name, char *format){

  int h,w,pitch;
  SDL_Surface *surface = NULL;
  Uint32 *pixels = NULL ; 
  SDL_GetWindowSize(window, &w , &h);
  SDL_Rect rect = {0, 0 , w , h};
  pitch = sizeof(Uint32) * rect.w ;
  pixels = malloc(pitch * rect.h);
  getPixels(renderer , &rect , pixels);
  surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels,rect.w,rect.h,32,pitch,SDL_PIXELFORMAT_RGBA8888);


  if(strcmp(format, "bmp")==0){
    SDL_SaveBMP(surface,strcat(name,format));
    SDL_FreeSurface(surface);
  }else{
    if(strcmp(format,"png")==0){
      char *s=NULL;
      s=(char *)malloc((strlen(name)+strlen(format)+1)*sizeof(char));
      strcpy(s,name);
      strcat(s,".");
      strcat(s,format);
      IMG_SavePNG(surface,s);
      SDL_FreeSurface(surface);
    }else{
      if(strcmp(format,"jpg")==0){
	printf("à venir");
	SDL_FreeSurface(surface);
      }else {

	return -1;
      }
    }
  }
  return 0;
}
