#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <pwd.h>
#include <unistd.h>
#include "CLI/cli.h"
#include "CLI/parsing.h"
#include "SHARED/shared.h"

#define H 900
#define W 920

/*---------------- Les Headers --------------------*/
short isCommand(char* text);
cmd *cmd_prompt();
int execution(cmd* c);
int exec_cmd(cmd* c);
void printInviteShell(char *curr_dir);
/*-------------------------------------------------*/

/* -------------- Variables Globale -------------- */
int nbCommands = 1;
formatCmd commands[] = { 
  {"loadimages", 0, 0}
};

/* Une variable où on sauvegarde la fenêtre ouverte actuallement */
char currentWindows[NAME_MAX] ;
int result = 0, busyWindows = 0;	
/*-------------------------------------------------*/

int main()
{ 
    int status = EXIT_FAILURE;
    cmd *c;
    if(SDL_Init(SDL_INIT_VIDEO != 0)){
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

  /* libération de la mémoire */
  //free(input_cmd);
  //free(curr_dir);  

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
  //char** args = c->args;
  //int nb_args = c->nb_args;

  if(strcmp(name, "loadimages") == 0){
    busyWindows = 1;
    strcpy(currentWindows, c->args[0]);
    SDL_Window *pWindow = NULL; 
    pWindow = SDL_CreateWindow("image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,600,600, SDL_WINDOW_SHOWN);
    
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
      fprintf(stdout, "\033[01;34m %s \033[00m >>> ", currentWindows );
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
