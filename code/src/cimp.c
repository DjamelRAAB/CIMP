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
void AfficherAide();
/*-------------------------------------------------*/

/* -------------- Variables Globale -------------- */
int nbCommands = 32;
formatCmd commands[] = {
  {OPEN_IMAGES, MAX_ARGS, 1},
  {CLOSE_IMAGES, MAX_ARGS, 1},
  {BASCULER_FENETRE, 1,1},
  {AFFICHER_LIST_FENETRES, 0, 0 },
  {SAVE_IMAGES, 3, 1 },
  {LOAD_IMAGE, 2, 1 },
  {NEGATIVE_COLOR, 0, 0},
  {GREY_COLOR, 0, 0},
  {FLIPVERTICAL, 0, 0},
  {FLIPHORIZONTAL, 0, 0},
  {BLACK_WHITE_COLOR, 0, 0},
  {BRIGHTNESS_ADJUST, 0, 0},
  {ADDCONTRAST, 0, 0},
  {SUBCONTRAST, 0, 0},
  {ROTATION_LEFT, 0, 0},
  {ROTATION_RIGHT, 0, 0},
  {SELECTAREA, 6,0},
  {DELIMITAREA, 5, 0},
  {DESELECTAREA, 2,1},
  {EDIT_PIXEL_IMAGE, 0, 0},
  {ADDAREA,0,0},
  {DELETEAREA,0,0},
  {COPIER,1,0},
  {COUPER,1,0},
  {COLLER,1,1},
  {SYMETRIQUE,0,1},  
  {RECADRAGE,0,1},
  {EDIT_SIZE_IMAGE,0,1},
  {FILING_IMAGE,0,1},
  {TREAT_EXTERN_FILE_IMAGE,0,1},
  {"help",0,0}
};

/* Une variable où on sauvegarde la fenêtre ouverte actuallement */
windows *listeWindows = NULL;
int posInTabFenetres = 0;
dataWindows *currentWindows;
int result = 0, busyWindows = 0;
SDL_Rect zoneSelection;	
/*-----------------------------------------------#define SUBCONTRAST "sub_contrast"
--*/

int main(){ 
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
/*------------------------------- Traitement ------------------------------------------*/
/*-------------------------------------------------------------------------------------*/

/* Vérifaication de la commande */
short isCommand(char* text, int nbArgs){
  for(int i = 0;i < nbCommands;i++){
    if(strcmp(text, commands[i].name) == 0 && (nbArgs <= commands[i].nbArgs)){
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
  }else{
    fprintf(stdout, "\n >>>>>>>> la commande \033[31;01m \" %s \" \033[00m introuvable ou incorrecte \n ", c->nameCmd);
    return -1;
  }
}

/* ----------  Éxecution des commandes ---------- */ 
int execution(cmd* c){
  char* name = c->nameCmd;
  char** args = c->args;
  int nb_args = c->nb_args;

  char *paths[] = { "assets/pictures/moto.bmp"};

  /* ------- Chargement et sauvegarde d'images --------*/ 
  if(strcmp(name, OPEN_IMAGES) == 0){ // Ouverture de images 
    if( openImages(paths, 1, &listeWindows, &posInTabFenetres) != 0) {
      currentWindows = listeWindows->data;
      //SDL_RaiseWindow(currentWindows->fenetre);
      busyWindows = 1;
    }

  }else if (strcmp(name, CLOSE_IMAGES) == 0) { // Fermeture des images ou une  l'image courante
    if(busyWindows == 1 ){
      if(nb_args == 0){
        closeImage(&currentWindows);
        listeWindows = deleteWindows(listeWindows, currentWindows->id);
      }
      else{
        windows *fe = foundWindows(listeWindows, atoi(args[0]));
        if(fe != NULL){
          closeImage(&fe->data);
          listeWindows = deleteWindows(listeWindows, fe->data->id);
        }
        else{
          printf("----------> Introuvable ID");
        }
      }
    }
    if(listeWindows != NULL){
      busyWindows = 1;
      currentWindows = listeWindows->data;
      //SDL_RaiseWindow(currentWindows->fenetre);
    }
    else busyWindows = 0;

  }else if (strcmp(name, BASCULER_FENETRE) == 0) { // Basculer vers une fenetre
    if ( busyWindows == 1){
      windows *fe = foundWindows(listeWindows, atoi(args[0]));
      if(fe != NULL){
        changerFenetre(listeWindows, &currentWindows, atoi(args[0]));
      }else{
        fprintf(stdout, ">>>>>>>>>>>> Erreur !!!");
      }
    }else{
      fprintf(stdout, ">>>>>>>>>>>> Erreur !!!");
    }

  }else if (strcmp(name, SAVE_IMAGES) == 0) { // sauvegarder une image 
    if ( busyWindows == 1){
      if(saveImage(currentWindows, "SAVE/savePicture","bmp")!=0){
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
  }

  /*------ Transformations ----- */
  else if (strcmp(name, NEGATIVE_COLOR) == 0){ // Rendre une image négative
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, NEGATIVE);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, NEGATIVE);
    }

  }else if (strcmp(name, GREY_COLOR) == 0 ){ // la mise en niveaux gris d'une image ou de la sélection courante
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, GREY);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, GREY);
    }

  }else if( strcmp(name, BLACK_WHITE_COLOR) == 0){ // la mise en noir et blanc d'une image ou de la sélection courante
    if ( busyWindows == 1){
      printf("-------> Noir et blanc ");
    }

  }else if(strcmp(name, BRIGHTNESS_ADJUST) == 0){
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      LuminositeEffect(&currentWindows, &rect, 5);
      LuminositeEffect(&currentWindows, &rect, 5);
    } 

  }else if(strcmp(name, ADDCONTRAST) == 0){
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      addContrastEffect(&currentWindows, &rect, 5);
      addContrastEffect(&currentWindows, &rect, 5);
    }
  }else if(strcmp(name, FLIPVERTICAL) == 0){
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, FLIP_VERTICAL);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, FLIP_VERTICAL);
    }

  }else if(strcmp(name, FLIPHORIZONTAL) == 0){
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, FLIP_HORIZONTAL);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, FLIP_HORIZONTAL);
    }

  }else if(strcmp(name, SUBCONTRAST) == 0){
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      subContrastEffect(&currentWindows, &rect, 5);
      subContrastEffect(&currentWindows, &rect, 5);
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
  
  /*------ Sélection ------- */
  }else if(strcmp(name, SELECTAREA) == 0){
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      selectRect(currentWindows->renderer);
    }

  }else if(strcmp(name, DELIMITAREA) == 0){
    if ( busyWindows == 1){
      printf("-----------> Deselection \n");
    }

  /*------ Copier, Couper, Coller ------- */
  }else if(strcmp(name, COPIER) == 0){
    if ( busyWindows == 1){
      printf("-----------> copier \n");
    }

  }else if(strcmp(name, COUPER) == 0){
    if ( busyWindows == 1){
      printf("-----------> couper \n");
    }
  }else if(strcmp(name, COLLER) == 0){
    if ( busyWindows == 1){
      printf("-----------> coller \n");
    }
  /* --------- Erreur --------- */
  }else if(strcmp(name, "help") == 0){
    printf("\t\t\t------------------ HELP ----------------- \n");
    AfficherAide();
  }else{
    perror("-mpsh: no such intern command");
    return 1; 
  }

  return 0;
}
/*-------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/

/*help*/
void AfficherAide(){
  printf("\033[01;34m open    \033[00m : Ouvrir une image et de charger en mémoire une ou plusieurs images\n"); 
  printf("\033[01;34m close   \033[00m : elle permet de fermer une fenetre\n"); 
  printf("\033[01;34m basculer\033[00m : cette fonctionalité permet de basculer entre les fenetre autrement dit passer d'une fenetre à une autre\n"); 
  printf("\033[01;34m liste   \033[00m : Afficher la liste des fenetres ouvertes\n"); 
  printf("\033[01;34m save    \033[00m : Sauvegarder une image\n");
  printf("loadImage : Télécharger une image\n"); 
  printf("selectArea : Selectionner une zone d'une image\n");
  printf("delimitArea : Delimiter une zone avec des pointiers\n");
  printf("deselectArea : Deselectionner une zone déjà selectionnée\n");
  printf("editPixelImage:                               \n");
  printf("addArea : Ajouter une zone à une image déjà existante\n");
  printf("deleteArea : Supprimer une zone d'une image existante\n");
  printf ("copier : Copier une région d'image \n"); 
  printf("couper : Couper une région d'image en remplaçant la zone selectionnée par une couleur de fond\n");
  printf("coller : Coller la zone copiée ou coupée dans n'importe quelle coordonnées de cette image ou d'une autre image\n");
  printf("symetrie : Effectuer sur une image les transformations de symétries verticale et horizontale \n");  
  printf("recadrage : Recadrage de l’image par découpage rectangulaire et par agrandissement de la zone de travail \n"); 
  printf("filling : Permet le remplissage par une couleur donnée qui s'appliqueront à une image ou à la sélection courante \n");
}