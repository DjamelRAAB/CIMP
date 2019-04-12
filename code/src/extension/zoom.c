#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "MINIMAL/select.h"


/**
 * La fonction zoom permet de zoumer sur une image et de scroller avec les fléches,
 * A pour zoumer Z pour dézoumer Q pour quiter 
 * @param renderer : le rendu sure le quel travailler.
 * @param e : l'evenement courant
 * @return int : 0 si tout va bien 1 sinon.
 **/
int zoom (SDL_Renderer *renderer , SDL_Event e){
  int i ,j , pitch ,pas =10 ,px=0,py=0; 
  int boo = 0 ;
  double coef , zoom= 0.0 ;
  SDL_Rect rect;
  SDL_Rect tmp ;
  Uint32 *pixels; 
  Uint32 *pixels2; 
  SDL_Texture *textureRect ; 
  SDL_RenderGetViewport(renderer, &tmp);//on récupère la zone de travaille dans rect
  SDL_RenderGetViewport(renderer, &rect);//on récupère la zone de travaille dans rect
  coef = (float)tmp.h/(float)tmp.w ;
  pitch = sizeof(Uint32) * tmp.w ;
  pixels = malloc(pitch * tmp.h);
  if(pixels == NULL){
    return 1;
  }
  getPixels(renderer , &tmp ,pixels);
  do{
    if (SDL_PollEvent(&e)) {
      if(e.type == SDL_KEYDOWN){
        switch(e.key.keysym.scancode ){
          case SDL_SCANCODE_UP :
            if (rect.y - pas > 0){
              py -= pas;
              rect.y -=pas;
            }
            break;
          case SDL_SCANCODE_LEFT :
            if (rect.x - pas > 0){
              px -= pas;
              rect.x -= pas;
            }
            break;
          case SDL_SCANCODE_DOWN :
            if (rect.y + pas + rect.h < tmp.h){
              py+=pas;
              rect.y += pas;
            }
            break;
          case SDL_SCANCODE_RIGHT :
            if (rect.x + pas + rect.w < tmp.w) {
              px+=pas;
              rect.x += pas;
            }
            break;
          case SDL_SCANCODE_A :
            if (boo){
              boo=px=py=zoom=0;
              rect.x=tmp.x;
              rect.y=tmp.y;
              rect.h=tmp.h;
              rect.w=tmp.w;
            }
            zoom+=10;
            rect.h = tmp.h - (zoom*coef);
            rect.w = tmp.w - zoom;
            rect.x = tmp.x +px + zoom/2;
            rect.y = tmp.y + py + (zoom*coef)/2;
            SDL_DestroyTexture(textureRect);
            textureRect = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, rect.w, rect.h );
            
            break;
          case SDL_SCANCODE_Z :
            if(zoom>0){
              zoom-=10;
              rect.h = tmp.h - (zoom*coef);
              rect.w = tmp.w - zoom;
              if(tmp.x + px + zoom/2 > 0 && tmp.x + px + zoom/2 < tmp.w)
                rect.x = tmp.x + px + zoom/2;
              else {
                rect.x = tmp.x + zoom/2;
                boo=1;
              }
              if(tmp.y + py + (zoom*coef)/2>0  && tmp.y + py + (zoom*coef)/2 < tmp.h)
                rect.y = tmp.y + py + (zoom*coef)/2;
              else{
                rect.y = tmp.y  + (zoom*coef)/2;
                boo=1;
              }              
              if(rect.x < 0){
                rect.x =0;
                boo= 1;
              }
              if(rect.y<0){
                rect.y = 0;
                boo=1;
              }
              if(rect.x + rect.w >tmp.w){
                rect.x=tmp.w-rect.w;
              //  boo = 1;
              }
              if(rect.y + rect.h >tmp.h){
              //  boo=1;
                rect.y=tmp.h-rect.h;
              }
              SDL_DestroyTexture(textureRect);
              textureRect = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, rect.w, rect.h );
            }
            break;
          default :
            break;
        } 
      }
    }
    if(e.type==SDL_KEYUP){
      pitch = sizeof(Uint32) * rect.w ;
      pixels2 = malloc(pitch * rect.h);   
      for(i = 0; i < rect.h; i++){
        for(j = 0; j < rect.w; j++){
          pixels2[i * rect.w + j] = pixels[(rect.y+i) * tmp.w + (rect.x+j)];
        }
      }
      SDL_UpdateTexture(textureRect, NULL, pixels2, sizeof(Uint32) * rect.w);
      SDL_RenderCopy(renderer, textureRect, NULL, NULL);
      SDL_RenderPresent(renderer);
      free(pixels2);
    }
  }while(e.key.keysym.scancode != SDL_SCANCODE_Q);
  SDL_DestroyTexture(textureRect);
  free(pixels);
  return 1 ; 
}