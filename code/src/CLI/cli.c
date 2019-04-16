#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <pwd.h>
#include "CLI/cli.h"
#include "CLI/parsing.h"


#define DEF_INVITE "\033[01;32m /u @ /h : \033[01;34m /d \033[00m /$  >>> " //valeur par defaut de INVITE


/****************** Les Headers *********************/
cmd *cmd_prompt();
void vider_string(char * s);
short isCommand(char* text);
int execution(cmd* c);
int exec_cmd(cmd* c);
void initShell();
void printInviteShell();
/****************************************************/

int nbCommands = 1;
formatCmd commands[] = { 
  {"loadimages", 0, 0}
};

/* Une variable où on sauvegarde la fenêtre ouverte actuallement */
char currentWindows[NAME_MAX] ;
int result = 0, busyWindows = 0;			

int main(void){

  cmd *c;
  initShell ();

  /*boucle d'intéraction avec l'utilisateur*/
  while(1){
    c=cmd_prompt();
    result = exec_cmd(c);
    free(c);
  }
  
  return 0;
}

/* Au lancement du Shell  */
void initShell(){
  printf("\033[H\033[J");
  printf("\n\n\n\n\t\t******************************************\n"); 
  printf("\n\t\t*************** CIMP *********************\n") ;
  printf("\n\t\t******************************************\n\n"); 
  sleep(1); 
  printf("\033[H\033[J");
}

short isCommand(char* text){
  for(int i = 0;i < nbCommands;i++){
    if(strcmp(text, commands[i].name) == 0){
      return 1;
    }
  }
  return 0;
}

int exec_cmd(cmd* c){
  if(isCommand(c->nameCmd)){
    return execution(c);
  }
  else{
    perror("Commandes introuvable");
    return -1;
  }
}

int execution(cmd* c){
  char* name = c->nameCmd;
  char** args = c->args;
  int nb_args = c->nb_args;

  if(strcmp(name, "loadimages") == 0){
    busyWindows = 1;
    strcpy(currentWindows, c->args[0]);
    
  }else{
    perror("-mpsh: no such intern command");
    return 1;
  }
  return 0;
}


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

void vider_string(char * s){
  strcpy(s,"");
}

