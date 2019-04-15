#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SHARED/shared.h>
#include <CLI/cli.h>

#define H 900
#define W 920

int main(int argc, char const *argv[])
{   
    SDL_Window *pWindow = NULL; 
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture_image = NULL;
    SDL_Color vert =  {0, 7, 227, 0};
    int status = EXIT_FAILURE;

    if(SDL_Init(SDL_INIT_VIDEO != 0)){
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
  
    if(init(&pWindow, &renderer, W, H) != 0 ){
        goto Quit;
    };
    
    /* Image */
    
    SDL_Surface *icon = NULL;  
    void *tmp;
    Uint32 *pixels;
    int pitch;

    icon = SDL_LoadBMP("assets/pictures/icon.bmp");
    SDL_SetWindowIcon(pWindow, icon);
    SDL_SetWindowTitle(pWindow,"Image");
    
    texture_image = loadImage("assets/pictures/moto.bmp",renderer);
    if(texture_image == NULL){
        goto Quit;
    }


    SDL_RenderCopy(renderer, texture_image, NULL, NULL);
    SDL_SetRenderDrawColor(renderer, vert.r,vert.g, vert.b, 10);
    SDL_RenderDrawLine(renderer,0,0, 500, 500);
    SDL_RenderPresent(renderer);
    SDL_Event e;
    int stat = 1;
    do
    {
        SDL_PollEvent(&e);
        switch (e.type)
        {
            case SDL_QUIT:
                printf(" fermer = %d \n ", e.type);
                stat = 0 ;
                break;
        
            default:
                printf("%d \n ", e.type);
                break;
        }
    } while (stat != 0);
    

    status = EXIT_SUCCESS;

Quit:
    if(texture_image != NULL)
        SDL_DestroyTexture(texture_image);   
    if(renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if(NULL != pWindow)
        SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return status;
}
