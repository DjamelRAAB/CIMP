#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "image_processing.h"
#include "select.h"

/**
 * Change la luminosité d'une couleur.
 * @param  c : la composante de la couleur à éclairecire. 
 * @param  n : le coefficient de luminosité à appliquer.
 * @return le nouvelle coulleur. 
 **/
Uint8 fBRIGHTNESS(Uint8 c, int n){
  return (Uint8) (255 * SDL_pow((double) c / 255, n));
}

/**
 * Charge le contraste d'une couleur.
 * @param  c : la composante de la couleur à éclairecire. 
 * @param  n : le coefficient de contraste à appliquer.  
 * @return le nouvelle coulleur. 
 **/
Uint8 fContrast(Uint8 c, double n){
  if(c <= 255 / 2)
    return (Uint8)( (255/2) * SDL_pow((double) 2 * c / 255, n));
  else
    return 255 - fContrast(255 - c, n);
}

/**
 * Rend un pixel flou
 * @param  pixels : le tabbleau de pixel dans le quele est situé le pixel à flouté. 
 * @param  rect : le rectangle qui défine le tableau pixels.
 * @param i j : les coordonnées du pixel a flouté.
 * @param  n : le coefficient du flou à appliquer.
 * @return Une SDL_Texture modifiable qui contient l'image qu'on veux chargé.  
 **/
Uint32 flouPixel(Uint32 * pixels, SDL_Rect *rect ,int i, int j, int n){
  const int initial_h = SDL_max(i - n, 0);
  const int initial_w = SDL_max(j - n, 0);
  const int final_h = SDL_min(i + n, rect->h - 1);
  const int final_w = SDL_min(j + n, rect->w - 1);
  const int nb_pixel = ((final_h - initial_h) * (final_w - initial_w));

  Uint32 sum_r = 0, sum_g = 0, sum_b = 0;
  SDL_Color color;

  for (i = initial_h; i < final_h; i++)
    for(j = initial_w; j < final_w; j++)
      {
	SDL_GetRGBA(pixels[i * rect->w + j],SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888) , &color.r, &color.g, &color.b,&color.a);
	sum_r += color.r;
	sum_g += color.g;
	sum_b += color.b;
      }
  return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), sum_r / nb_pixel, sum_g / nb_pixel, sum_b / nb_pixel, color.a);
}

/**
 * Applique un traitement à une parti ou la totalité de l'image.
 * @param  renderer : le rendu de la fenétre.
 * @param  rect : la parti à traiter. 
 * @param c : la couleur a appliquer si l'act est FILLING.
 * @param act : l'action a faire sur la selection.
 * @return retourne 0 si tout c'est bien passer 1 si non. 
 **/
int imageProcessing (SDL_Renderer *renderer , SDL_Rect *rect , SDL_Color c , enum Processing act){
  int i , j ,n = 2;
  int pitch ;
  Uint32 *pixels; 
  Uint32 *pixels2; 
  SDL_Texture *texture ; 
  SDL_PixelFormat *format;
  SDL_Color tmp ;

  pitch = sizeof(Uint32) * rect->w ;

  pixels = malloc(pitch * rect->h);
  if(pixels == NULL){
    return 1;
  }

  pixels2 = malloc(pitch * rect->h);
  if(pixels2 == NULL){
    free(pixels);
    return 1;
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, rect->w, rect->h );
  if(texture == NULL){
    free(pixels);
    return 1;
  }

  getPixels(renderer , rect , pixels );
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  
  switch (act){

  case FILLING:
    for(i = 0; i < rect->h; i++){
      for(j = 0; j < rect->w; j++){
	pixels[i * rect->w + j] = SDL_MapRGBA(format,c.r, c.g, c.b ,c.a);
      }
    }
    break;

  case GREY:
    for(i = 0; i < rect->h; i++){
      for(j = 0; j < rect->w; j++){
	SDL_GetRGBA(pixels[i * rect->w + j], format, &tmp.r, &tmp.g, &tmp.b , &tmp.a );
	Uint8 gris = (tmp.r + tmp.g + tmp.b) / 3;
	pixels[i * rect->w + j] = SDL_MapRGBA(format, gris, gris, gris ,tmp.a);
      }
    }
    break;

  case NEGATIVE:
    for(i = 0; i < rect->h; i++){
      for(j = 0; j < rect->w; j++){
	SDL_GetRGBA(pixels[i * rect->w + j], format, &tmp.r, &tmp.g, &tmp.b , &tmp.a );
	pixels[i * rect->w + j] = SDL_MapRGBA(format, 255-tmp.r, 255-tmp.g, 255-tmp.b ,tmp.a);
      }    
    }
    break;

  case BRIGHTNESS:
    for(i = 0; i < rect->h; i++){
      for(j = 0; j < rect->w; j++){
	SDL_GetRGBA(pixels[i * rect->w + j], format, &tmp.r, &tmp.g, &tmp.b , &tmp.a );
	tmp.r = fBRIGHTNESS(tmp.r, n);
	tmp.g = fBRIGHTNESS(tmp.g, n);
	tmp.b = fBRIGHTNESS(tmp.b, n);
	pixels[i * rect->w + j] = SDL_MapRGBA(format, tmp.r, tmp.g, tmp.b ,tmp.a);
      }
    }  
    break;

  case ADD_CONTRAST:
    for(i = 0; i < rect->h; i++){
      for(j = 0; j < rect->w; j++){
	SDL_GetRGBA(pixels[i * rect->w + j], format, &tmp.r, &tmp.g, &tmp.b , &tmp.a );
	tmp.r = fContrast(tmp.r, n);
	tmp.g = fContrast(tmp.g, n);
	tmp.b = fContrast(tmp.b, n);
	pixels[i * rect->w + j] = SDL_MapRGBA(format, tmp.r, tmp.g, tmp.b ,tmp.a); 
      }
    }
    break;

  case SUB_CONTRAST:
    for(i = 0; i < rect->h; i++){
      for(j = 0; j < rect->w; j++){
	SDL_GetRGBA(pixels[i * rect->w + j], format, &tmp.r, &tmp.g, &tmp.b , &tmp.a );
	tmp.r = fContrast(tmp.r, -n);
	tmp.g = fContrast(tmp.g, -n);
	tmp.b = fContrast(tmp.b, -n);
	pixels[i * rect->w + j] = SDL_MapRGBA(format, tmp.r, tmp.g, tmp.b ,tmp.a);  
      }
    }
    break;

  case BLURRED:                              
    for(i = 0; i < rect->h; i++){
      for(j = 0; j < rect->w; j++){
        SDL_GetRGBA(pixels[i * rect->w + j], format, &tmp.r, &tmp.g, &tmp.b , &tmp.a );
        pixels2[i * rect->w + j] = flouPixel(pixels,rect,i,j,n); 
      }
    }
    break;                              

  case CLIPPING:
    for(i = 0; i < rect->h; i++){
      for(j = 0; j < rect->w; j++){
	if(i==0||i==rect->h-1||j==0||j==rect->w-1){
	  SDL_GetRGBA(pixels[i * rect->w + j], format, &tmp.r, &tmp.g, &tmp.b , &tmp.a );
	  pixels2[i * rect->w + j] = 255 - SDL_abs(pixels[i * rect->w + j] - flouPixel(pixels,rect,i,j,n));
	}
      }
    }
    break;                              
    
  default:
    printf("Aucun traitement disponible pour l'act demander!");
    return 1; 
    break;
  }
  
  SDL_FreeFormat(format);
  if(act==CLIPPING || act==BLURRED)
    SDL_UpdateTexture(texture, NULL, pixels2, sizeof(Uint32) * rect->w);
  else
    SDL_UpdateTexture(texture, NULL, pixels, sizeof(Uint32) * rect->w);
  SDL_RenderCopy(renderer, texture, NULL, rect);
  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(texture);
  free(pixels);
  free(pixels2);
  return 0 ;
}
