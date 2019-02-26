#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char const *argv[])
{   
    SDL_Window *pWindow = NULL; 
    SDL_Renderer *renderer = NULL;
    int status = EXIT_FAILURE;

    /* Initialisation simple */
    if (SDL_Init(SDL_INIT_VIDEO) != 0 ){
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        goto Quit;  
    }

    /* Création de la fenêtre */    
    pWindow = SDL_CreateWindow("Ma première application SDL2",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                640, 480, SDL_WINDOW_RESIZABLE);
    if( pWindow == NULL ){
        fprintf(stderr, "Erreur de creation de la fennetre : %s \n", SDL_GetError());
        goto Quit;
    }
    renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL){
        fprintf(stderr, "Erreur à la creation de Renderer : %s \n", SDL_GetError());
        goto Quit;
    }
    
    /* Dessin */
    SDL_SetRenderDrawColor(renderer,255,127,40,255);
    SDL_RenderClear(renderer);

    SDL_Delay(1000);
    SDL_RenderPresent(renderer);
    SDL_Delay(8000);
    
    status = EXIT_SUCCESS;

Quit: 
    if(renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if(NULL != pWindow)
        SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return status;
}
