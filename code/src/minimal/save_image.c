#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "save_image.h"



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
  pitch = sizeof(Uint32) * w ;
  pixels = malloc(pitch *h);
  SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, pixels, pitch);
  surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels,w,h,32,pitch,SDL_PIXELFORMAT_RGBA8888);


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
