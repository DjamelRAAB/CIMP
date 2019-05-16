#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <pwd.h>
#include <unistd.h>

#include "cli/cli.h"
#include "cli/parsing.h"
#include "shared/shared.h"
#include "windows/windows.h"
#include "../include/defaultValues.h"
#include "minimale/openSaveImages.h"
#include "minimale/select.h"
#include "minimale/image_processing.h"

/*---------------- Les Headers --------------------*/
short isCommand(char* text, int nbArgs);
cmd *cmd_prompt();
int execution(cmd* c);
int exec_cmd(cmd* c);
void printInviteShell(char *curr_dir);
/*-------------------------------------------------*/

/* -------------- Variables Globale -------------- */
int nbCommands = 12;
formatCmd commands[] = { 
  {OPEN_IMAGES, MAX_ARGS, 1},
  {CLOSE_IMAGES, MAX_ARGS, 1},
  {BASCULER_FENETRE, 1,1},
  {AFFICHER_LIST_FENETRES, 0, 0 },
  {SAVE_IMAGES, 2, 0 },
  {LOAD_IMAGE, 1, 1 },
  {NEGATIVE_EFFECT, 0, 0},
  {GREY_EFFECT, 0, 0},
  {NOIRBLAN, 0, 0},
  {BRIGHTNESS_EFFECT, 1, 1},
  {ROTATION_LEFT, 0, 0},
  {ROTATION_RIGHT, 0, 0}
};

/* Une variable où on sauvegarde la fenêtre ouverte actuallement */
windows *listeWindows = NULL;
int posInTabFenetres = 0;
dataWindows *currentWindows;
int result = 0, busyWindows = 0;
SDL_Rect zoneSelection;	
/*-------------------------------------------------*/

int main()
{ 
    int status = EXIT_FAILURE;
    cmd *c;
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    
    initShell ();

    /* Boucle d'intéraction avec l'utilisateur */
    while(1){
      c=cmd_prompt();
      result = exec_cmd(c);
      free(c);
    }

    status = EXIT_SUCCESS;
Quit:
    SDL_Quit();
    return status;
}

/*------------------------------------ CLI --------------------------------------------*/
/*-------------------------------------------------------------------------------------*/
/* Fonction pour la lecture de la commande sur le shell */
cmd *cmd_prompt(){
  char *curr_dir=malloc(BUF_LENGTH*sizeof(char));
  char *input_cmd=malloc(BUF_LENGTH*sizeof(char));
  cmd *c;

  getcwd(curr_dir,BUF_LENGTH);  // Récuprer le nom du répertoire currant 
  printInviteShell(curr_dir);   // Affichage de l'invite de commmande  
  fgets(input_cmd,BUF_LENGTH,stdin);  // Écriture de la commande par l'utilisateur

  char* n = strchr(input_cmd, '\n'); //suppression de \n
  if(n != NULL){
    n[0] = '\0';
  }

  c = CreateCmd(input_cmd);

  return c; // Retourner une commande structurée 
}

/* L'affichage de l'invite de commande */
void printInviteShell(char *curr_dir){
  fprintf(stdout,"\n");
  
  if (busyWindows != 0 ) {
      fprintf(stdout, "\033[01;34m %s \033[00m >>> ", currentWindows->path );
  }
  else{
    char str[BUF_LENGTH]= DEF_INVITE;
    char* token = strtok(str, " "); 
    while(token != NULL){

        if (strcmp(token, "/u") == 0){ //user
        uid_t uid = geteuid();
        struct passwd *pw = getpwuid(uid);
            fprintf(stdout, "%s", pw->pw_name);
        }
        else if( strcmp(token, "/h") == 0){ //hostname
        char name[256];
        gethostname(name, 256);
        fprintf(stdout, "%s", name);
        }
        else if (strcmp(token, "/d") == 0){ // repertoire
        fprintf(stdout, "%s", curr_dir);
        }
        else if (strcmp(token, "/$") == 0){ // simple user ou root
        if( geteuid() == 0 ){
            fprintf(stdout, "%s ", "#");
        }
        else{
            fprintf(stdout, "%s ", "$");
        }
        }
        else { // autre
        fprintf(stdout,"%s",token);
        }
        token = strtok(NULL," ");
    }
  }

  fprintf(stdout," ");
}


/*-------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/
/*------------------------------- Traitement ------------------------------------------*/
/*-------------------------------------------------------------------------------------*/

/* Vérifaication de la commande */
short isCommand(char* text, int nbArgs){
  for(int i = 0;i < nbCommands;i++){
    if(strcmp(text, commands[i].name) == 0 && ( nbArgs <= commands[i].nbArgs ) ){
      if(nbArgs == 0 && commands[i].requiredArgs == 1)
        return 0;
      else 
        return 1;
    }
  }
  return 0;
}


/* Vérification des commandes */
int exec_cmd(cmd* c){
  if(isCommand(c->nameCmd, c->nb_args)){
    return execution(c);
  }
  else{
    fprintf(stdout, "\n >>>>>>>> la commande \033[31;01m \" %s \" \033[00m introuvable ou incorrecte \n ", c->nameCmd);
    return -1;
  }
}

/* Éxecution des commandes */ 
int execution(cmd* c){
  char* name = c->nameCmd;
  char** args = c->args;
  int nb_args = c->nb_args;

  char *paths[] = { "assets/pictures/moto.bmp"};

  if(strcmp(name, OPEN_IMAGES) == 0){ // Ouverture de images 
    if( openImages(paths, 1, &listeWindows, &posInTabFenetres) != 0) {
      currentWindows = listeWindows->data;
      busyWindows = 1;
    }

  }else if (strcmp(name, CLOSE_IMAGES) == 0) { // Fermeture des images
    if(busyWindows == 1 ){
      closeImage(&currentWindows);
      listeWindows = deleteWindows(listeWindows, currentWindows->id);
    }
    if(listeWindows != NULL){
      busyWindows = 1;
      currentWindows = listeWindows->data;
      SDL_RaiseWindow(currentWindows->fenetre);
    }
    else busyWindows = 0;

  }else if (strcmp(name, BASCULER_FENETRE) == 0) { // Basculer vers une fenetre
    if ( busyWindows == 1){
      changerFenetre(listeWindows, &currentWindows, atoi(args[0]));
    }
    else{
      fprintf(stdout, ">>>>>>>>>>>> Erreur !!!");
    }
    
  }else if (strcmp(name, SAVE_IMAGES) == 0) { // sauvegarder  une image 
    if ( busyWindows == 1){
      if(saveImage(currentWindows, "SAVE/savePicture","png")!=0){
        fprintf(stderr, "Erreur saveImage : %s", SDL_GetError());
      }
    }
    else{
      fprintf(stdout, ">>>>>>>>>>>> Erreur !!!");
    }
  }else if (strcmp(name, LOAD_IMAGE) == 0) { // chargement d'une image 
    if ( busyWindows == 1){
      if(loadImageWindows(&listeWindows,&currentWindows,&posInTabFenetres, args[0], args[1])!=0){
        fprintf(stderr, "Erreur saveImage : %s", SDL_GetError());
      }
    }
    else{
      fprintf(stdout, ">>>>>>>>>>>> Erreur !!!");
    }
  }else if (strcmp(name, AFFICHER_LIST_FENETRES) == 0) { // afficher la liste des fenetres
    printWindowsList(listeWindows);
  }else if (strcmp(name, NEGATIVE_EFFECT) == 0){ // Rendre une image négative
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, NEGATIVE);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, NEGATIVE);
      //SDL_RenderPresent(currentWindows->renderer);
    }
  }else if (strcmp(name, GREY_EFFECT) == 0 ){ // la mise en niveaux gris d'une image ou de la sélection courante
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, GREY);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, GREY);
    }
  }else if( strcmp(name, NOIRBLAN) == 0){ // la mise en noir et blanc d'une image ou de la sélection courante
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, NEGATIVE);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, NEGATIVE);
    }
  }else if(strcmp(name, BRIGHTNESS_EFFECT) == 0){
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      LuminositeEffect(&currentWindows, &rect, atoi(args[0]));
      //LuminositeEffect(&currentWindows, &rect, atoi(args[0]));
    } 
  }else if(strcmp(name, ROTATION_LEFT) == 0){
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, LEFT_ROTATION);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, LEFT_ROTATION);
    }
  }else if(strcmp(name, ROTATION_RIGHT) == 0){
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, RIGHT_ROTATION);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, RIGHT_ROTATION);
    }
  }else{
    perror("-mpsh: no such intern command");
    return 1; 
  }
  return 0;
}


/*-------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/
