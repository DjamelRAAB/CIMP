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
cmd *command(char * input);
cmd *cmd_prompt();
void vider_string(char * s);
short isIntern(char* text);
int exec_intern(cmd* c);
/*int exec_extern(cmd* c, int* rp, int* wp);
int exec_ench(cmd* c);*/
int exec_cmd(cmd* c);
void initShell();
void printInviteShell();
/****************************************************/

int nbCommands = 1;
char* commands[] = { 
    "loadimages" 
};

char* currentWindows = NULL;

int result = 0;			
int saved_stdout;
int saved_stdin;

int main(void){

  cmd *c = malloc(sizeof(cmd));
  initShell ();
  //boucle d'intéraction avec l'utilisateur
  //pour en sortir pour le moment cntrl c
  while(1){
    c=cmd_prompt();
    //printf("\n cmd = %s, nbArgs = %d , args : %s ", (c->args)[0], c->nb_args, (c->args)[1] );
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

short isIntern(char* text){
  for(int i = 0;i < nbCommands;i++){
    if(strcmp(text, commands[i]) == 0){
      return 1;
    }
  }
  return 0;
}

int exec_cmd(cmd* c){
  printf("\n -%s-, -%s-, %i\n",c->args[0], c->args[1], c->nb_args);
  if(isIntern(c->args[0])){
    return exec_intern(c);
  }
  else{
    perror("Commandes introuvable");
    return -1;
  }
}

int exec_intern(cmd* c){
  char* name = c->args[0];
  char** args = c->args;
  int nb_args = c->nb_args;

  if(strcmp(name, "loadimages") == 0){
    currentWindows = c->args[0];
    fprintf(stdout," succes !!!");
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
  cmd *c = malloc(sizeof(cmd));
  
  getcwd(curr_dir,BUF_LENGTH);

  printInviteShell(curr_dir);
  
  fgets(input_cmd,BUF_LENGTH,stdin);
  //suppression de \n
  char* n = strchr(input_cmd, '\n');
  if(n != NULL){
    n[0] = '\0';
  }

  c = CreateCmd(input_cmd);
  free(input_cmd);
  free(curr_dir);  
  return (c);
}

/* L'affichage de l'invite de commande */
void printInviteShell(char *curr_dir){
  fprintf(stdout,"\n");
  
  if (currentWindows != NULL) {
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

