#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>
#include "../windows/windows.h"
#include "../shared/shared.h"
#include "openSaveImages.h"


/**
 * 
 * 
**/

int openImages(char *path[], int nbImages, windows *tabFenetres[], int *posCurrent){
    int savePos = *posCurrent, success = 1;
    for(int i = 0; i < nbImages && i < MAX_WINDOWS ; i++){
        success = 1;
        windows *w = initWindows(path[i]);
        SDL_Texture *textureImage = NULL;
        if( init(&(w->fenetre), &(w->renderer), W, H) == -1 ){
            success = 0;
            goto erreur;
        }
        SDL_SetWindowTitle(w->fenetre, w->path);
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
        tabFenetres[i] = w;
        (*posCurrent)++;
        success = 1;

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
void closeImage(windows **w){
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
