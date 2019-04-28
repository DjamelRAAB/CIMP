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

int openImages(char *path[], int nbImages, windows **tetelisteWindows, int *posCurrent){
    int savePos = *posCurrent, success = 1;
    for(int i = 0; i < nbImages && i < MAX_WINDOWS ; i++){
        success = 1;
        dataWindows *w = initWindows(path[i]);
        SDL_Texture *textureImage = NULL;

        w->fenetre = SDL_CreateWindow(w->path, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, 0);
        if( w->fenetre == NULL ){
            success = 0;
            goto erreur;
        }
        w->renderer = SDL_CreateRenderer(w->fenetre, -1, SDL_RENDERER_ACCELERATED);
        if( w->renderer == NULL ){
            success = 0;
            goto erreur;
        }

        fprintf(stdout, "----Essaie Ouverture de l'image %s \n ", w->path );
        textureImage = loadImage(w->path, (w->renderer));
        if( textureImage == NULL){
            success = 0;
            goto erreur;
        }
        fprintf(stdout, " \t ---- Ouverture de l'image %s, sont identifiant est : %d \n ", w->path, *posCurrent );
        SDL_RenderCopy(w->renderer, textureImage, NULL, NULL);
        SDL_RenderPresent(w->renderer);
        w->id = *posCurrent;
        *tetelisteWindows = addWindows(*tetelisteWindows, w);
        (*posCurrent)++;
        success = 1;

        SDL_MinimizeWindow(w->fenetre);

        erreur:
            if(textureImage != NULL && success == 0)
                SDL_DestroyTexture(textureImage);   
            if(w->renderer != NULL && success == 0)
                SDL_DestroyRenderer(w->renderer);
            if(NULL != w->fenetre && success == 0)
                SDL_DestroyWindow(w->fenetre);
    }
    if(*posCurrent == savePos) return 0; 
    else return 1;
}

/**
 * Fermeture de la fenêtre  
**/
void closeImage(dataWindows **w){
    SDL_DestroyRenderer( (*w)->renderer);
    SDL_DestroyWindow( (*w)->fenetre);
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