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
    windows *w = initWindows(path);
    SDL_Renderer *renderer = NULL;
    SDL_Texture *textureImage = NULL;
    SDL_Color blanc = {255, 255, 255, 255};
    if( init(&(w->fenetre), &renderer, W, H) == -1  )
        goto erreur;
    SDL_SetWindowTitle(w->fenetre, w->path);

    textureImage = loadImage(w->path, renderer);
    if( textureImage == NULL){
        goto erreur;
    }
    setWindowColor(renderer, blanc);
    SDL_RenderCopy(renderer, textureImage, NULL, NULL);
    SDL_RenderPresent(renderer);
    return w;

erreur:
    if(textureImage != NULL)
        SDL_DestroyTexture(textureImage);   
    if(renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if(NULL != w->fenetre)
        SDL_DestroyWindow(w->fenetre);
    return NULL;
}

/**
 * Fermeture de la fenêtre  
**/
void closeImage(windows *w){
    SDL_DestroyWindow(w->fenetre);
    free(w);
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
    tmp = SDL_LoadBMP(path);
    if(NULL == tmp)
    {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if(NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;

}