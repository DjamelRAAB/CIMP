#include <SDL2/SDL.h>

#ifndef __WINDOWS__H__
#define __WINDOWS__H__

#define H 900
#define W 920

#define MAX_WINDOWS 5
#define MAX_PATH 256
#define MAX_FORMAT 64

typedef struct windows{
    SDL_Window *fenetre; // la fenetre en SDL2
    SDL_Renderer *renderer; // Le renderer de la fenetre
    int id;              // l'identifiant de la fenetre
    SDL_Color color;     // la couleur du fond
    char formatImage[MAX_FORMAT];   // le format de l'image
    char path[MAX_PATH];          // le chemin de l'image
    int reduite;         // La fenêtre est en arriere plan ou pas (0 non reduite 1 sinon ) 
    int fermee;          // la fenêtre est fermée ou pas (0 non fermée 1 sinon )
}windows;


/* ---------------------------------------------------------*/

windows *initWindows(const char path[]);


#endif