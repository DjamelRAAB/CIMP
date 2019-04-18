#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <pwd.h>
#include <unistd.h>
#include "cli/cli.h"
#include "cli/parsing.h"
#include "shared/shared.h"
#include "windows/windows.h"
#include "minimale/openSaveImages.h"

/*---------------- Les Headers --------------------*/
short isCommand(char* text);
cmd *cmd_prompt();
int execution(cmd* c);
int exec_cmd(cmd* c);
void printInviteShell(char *curr_dir);
/*-------------------------------------------------*/

/* -------------- Variables Globale -------------- */
int nbCommands = 2;
formatCmd commands[] = { 
  {"openimages", 0, 0},
  {"closeimages", 0,0}
};

/* Une variable où on sauvegarde la fenêtre ouverte actuallement */
windows fenetres[MAX_WINDOWS];
windows *currentWindows;
int result = 0, busyWindows = 0;	
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

/*--------------------------------------------------------------------------*/

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

/* Vérifaication de la commande */
short isCommand(char* text){
  for(int i = 0;i < nbCommands;i++){
    if(strcmp(text, commands[i].name) == 0){
      return 1;
    }
  }
  return 0;
}


/* Vérification des commandes */
int exec_cmd(cmd* c){
  if(isCommand(c->nameCmd)){
    return execution(c);
  }
  else{
    perror("Commandes introuvable");
    return -1;
  }
}

/* Éxecution des commandes */ 
int execution(cmd* c){
  char* name = c->nameCmd;
  char** args = c->args;
  int nb_args = c->nb_args;

  if(strcmp(name, "openimages") == 0){
    currentWindows = openImages(args[0]);
    if( currentWindows != NULL)
      busyWindows = 1;
  }else if (strcmp(name, "closeimages") == 0) {
    if(busyWindows == 1 ){
      busyWindows = 0;
      closeImage(currentWindows);
    }
  }else{
    perror("-mpsh: no such intern command");
    return 1;
  }
  return 0;
}

/* L'affichage de l'invite de commande */
void printInviteShell(char *curr_dir){
  fprintf(stdout,"\n");
  
  if (busyWindows != 0 ) {
      fprintf(stdout, "\033[01;34m %d \033[00m >>> ", currentWindows->id );
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


/*--------------------------------------------------------------------------*/
