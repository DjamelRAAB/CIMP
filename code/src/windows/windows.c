#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "windows.h"

/**
 *  
 * 
**/

dataWindows *initWindows(const char path[]){
    SDL_Color defaultColor = {0,0,0,255}; 
    SDL_Window *f = NULL;
    SDL_Renderer *r = NULL;

    dataWindows *w = malloc(sizeof(dataWindows));
    sprintf(w->path, "%s", path);
    w->color = defaultColor;
    w->reduite = 0;
    w->fermee = 0;
    w->fenetre = f;
    w->renderer = r;
    w->id = 0;

    return w; 
}

/**
 * 
 **/

windows * addWindows(windows* teteListe, dataWindows *data){
    if( foundWindows(teteListe, data->id) != NULL ){
        return teteListe;
    }

    windows *w = malloc(sizeof(windows));
    w->data = data;
    w->next = NULL;

    if(teteListe == NULL){
        return w;
    }
    windows *tmp = teteListe;
    while(tmp->next){
        tmp = tmp->next;
    }
    tmp->next = w;

    return teteListe;
}

/**
 * 
**/
windows *foundWindows(windows *w, int id){
    int found = 0;
    windows *tmp = w;
    while(tmp != NULL && found == 0){
        if(tmp->data->id == id ){
            found = 1 ;
            return tmp;
        }
        tmp = tmp->next;
    }
    
    return NULL;
}

/**
 * 
**/
windows *deleteWindows(windows *w, int id){
    if (foundWindows(w, id) == NULL){
        return w;
    }
    windows *tmp, *saveTmp;
    if(w->data->id == id ){
        tmp = w->next;
        free(w);
        return tmp;
    }

    saveTmp = w;
    tmp = w->next;
    int stop = 0;
    while(!stop){
        if(tmp->data->id == id ){
            stop = 1;
            saveTmp->next = tmp->next;
            free(tmp);
        }
        saveTmp = tmp;
        tmp = tmp->next;
    }

    return w;
    
} 

/**
 * 
**/
void printWindowsList(windows *w){
    windows *tmp = w;
    while(tmp != NULL){
        
        printf("\n image = %s ==========> id = %d  \n", tmp->data->path ,tmp->data->id);

        tmp = tmp->next;
    }
}

/**
 * 
**/
int changerFenetre(windows *teteListe, dataWindows **currentWindows, int id){
    if( teteListe == NULL || currentWindows == NULL ){
        return 0;
    }
    windows *tmp = foundWindows(teteListe, id);
    if(tmp == NULL){
        fprintf(stdout, "incorrecte ID");
        return 0;
    }
    (*currentWindows)->reduite = 1;
    SDL_MinimizeWindow((*currentWindows)->fenetre);
    *currentWindows = tmp->data;
    SDL_RaiseWindow((*currentWindows)->fenetre);

    return 1;

}