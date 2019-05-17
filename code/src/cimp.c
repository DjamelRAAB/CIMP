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
int nbCommands = 27;
formatCmd commands[] = { 
  {OPEN_IMAGES, MAX_ARGS, 1},
  {CLOSE_IMAGES, MAX_ARGS, 1},
  {BASCULER_FENETRE, 1,1},
  {AFFICHER_LIST_FENETRES, 0, 0 },
  {SAVE_IMAGES, 3, 1 },
  {LOAD_IMAGE, 2, 1 },
  {NEGATIVE_COLOR, 0, 0},
  {GREY_COLOR, 0, 0},
  {BLACK_WHITE_COLOR, 0, 0},
  {BRIGHTNESS_ADJUST, 0, 0},
  {ROTATION_LEFT, 0, 0},
  {ROTATION_RIGHT, 0, 0},
  {SELECT_AREA, 6,1},
  {DELIMIT_AREA, 5, 1},
  {DESELECT_AREA, 2,1},
  {EDIT_PIXEL_IMAGE, },
  {ADD_AREA, ,1},
  {DELETE_AREA, ,1},
  {COPIER , ,1},
  {COUPER, ,1},
  {COLLER, ,1},
  {SYMETRIE, ,1},  
  {RECADRAGE, ,1},
  {EDIT_SIZE_IMAGE, ,1},
  {FILLING_IMAGE, ,1},
  {TREAT_EXTERN_FILE_IMAGE, ,1},
  {"help",0,0}
  {} 


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
      negativeEffect(&currentWindows, &rect);
    }
  }else if (strcmp(name, GREY_EFFECT) == 0 ){ // la mise en niveaux gris d'une image ou de la sélection courante
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, GREY);
    }
  }else if( strcmp(name, NOIRBLAN) == 0){ // la mise en noir et blanc d'une image ou de la sélection courante
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, NEGATIVE);
    }
  }else if(strcmp(name, BRIGHTNESS_EFFECT) == 0){
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, BRIGHTNESS);
    }
  }else if(strcmp(name, ROTATION_LEFT) == 0){
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, LEFT_ROTATION);
    }
  }else if(strcmp(name, ROTATION_RIGHT) == 0){
    if ( busyWindows == 1){
      SDL_Rect rect;
      SDL_RenderGetViewport(currentWindows->renderer, &rect);
      imageProcessing(&currentWindows->renderer, &rect, currentWindows->color, RIGHT_ROTATION);
    }
  else if(strcmp(name, help) == 0){
     if ( busyWindows == 1){
       AfficherAide();
     }
  }else{
    perror("-mpsh: no such intern command");
    return 1; 
  }
  return 0;
}

/*help*/
void AfficherAide(){
  printf("openImage: la commande permet d'ouvrir une image et de charger en mémoire une ou plusieurs
 images\n"); 
 printf("closeImage : elle permet de fermer une fenetre\n "); 
 printf(" basculerFenetre: cette fonctionalité permet de basculer entre les fenetre autrement dit passer d'une fenetre à une autre\n"); 
 printf("afficherListFenetres : cette commande permet d'afficher la liste des fenetres ouvertes\n"); 
 printf("saveImage : cette commande permet de sauvegarder une image\n");
 printf("loadImage : cette commande permet de télécharger une image\n"); 
 printf("selectArea : cette commande permet de selectionner une zone d'une image\n");
 printf("delimitArea : cette commande permet de delimiter une zone avec des pointiers\n");
 printf("deselectArea : cette commande permet de deselectionner une zone déjà selectionnée\n");
 printf("editPixelImage:                               \n");
 printf("addArea : cette commande permet d'ajouter une zone à une image déjà existante\n");
 printf("deleteArea : cette commande permet de supprimer une zone d'une image existante\n");
 printf ("copier : cette commande permet de copier une région d'image \n"); 
 printf("couper : cette commande permet de couper une région d'image en remplaçant la zone selectionnée par une couleur de fond\n");
 printf("coller : cette commande permet de coller la zone copiée ou coupée dans n'importe quelle coordonnées de cette image ou d'une autre image\n");
 printf("symetrie : cette commande permet d'effectuer sur une image les transformations de symétries verticale et horizontale \n");  
 printf("recadrage : cette commande recadrage de l’image par découpage rectangulaire et par agrandissement de la zone de travail \n"); 
 printf("filling : cette commande  permet le remplissage par une couleur donnée qui s'appliqueront à une image ou à la sélection courante \n");
}


/*-------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/
