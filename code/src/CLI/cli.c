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
/*int exec_intern(cmd* c, int* rp, int* wp);
int exec_extern(cmd* c, int* rp, int* wp);
int exec_ench(cmd* c);
int exec_cmd(cmd* c, int* rp, int* wp);*/
void initShell();
void printInviteShell();
/****************************************************/

int nbCommands = 1;
char* commands[] = { 
    "loadimages" 
};

char* currentWindows = NULL;

int last_return = 0;			
int saved_stdout;
int saved_stdin;

int main(void){

  cmd *c = malloc(sizeof(cmd));
  initShell ();
  //boucle d'intéraction avec l'utilisateur
  //pour en sortir pour le moment cntrl c
  while(1){
    c=cmd_prompt();
    printf("\n cmd = %s, nbArgs = %d , args : %s ", (c->args)[0], c->nb_args, (c->args)[1] );
    /*last_return = exec_ench(c);*/
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

/*
int exec_ench(cmd* c){
  cmd* p = c;
  int ret = 0;
  //garder la sortie du pipe precedent
  //pour remettre l'entr�e standard.
  //-1 s'il y avait pas de pipe
  int* lp = NULL;
  while(p != NULL){
    char* link = p->link_to_next;
    if(link == NULL){
      ret = exec_cmd(p, lp, NULL);
      break;
    }
    else if(strcmp(link, ">") == 0){
      if(p->next == NULL){
	fprintf(stderr, "-mpsh: erreur de syntaxe '>'\n");
        ret = -1;
	break;
      }
      int fd = open(p->next->args[0], O_RDWR|O_CREAT,
		    S_IWUSR | S_IRUSR);
      if(fd < 0){
	ret = 1;
	break;
      }
      dup2(fd, 1);
      ret = exec_cmd(p, NULL, NULL);
 
      if(lp != NULL){
        close(lp[0]);
	close(lp[1]);
	lp = NULL;
	dup2(saved_stdin, 0);
      }
      
      close(fd);
      dup2(saved_stdout, 1);
      break;
    }
    else if(strcmp(link, "<") == 0){
      if(p->next == NULL){
	fprintf(stderr, "-mpsh: erreur de syntaxe '<'\n");
	ret = -1;
	break;
      }
      int fd = open(p->next->args[0], O_RDWR,
		    S_IWUSR | S_IRUSR);
      if(fd < 0) {
	fprintf(stderr, "-mpsh: %s n'existe pas\n",
		p->next->args[0]);
        ret = 1;
	break;
      }

      //pipe precedent et "<" -> pas de sens
      if(lp != NULL){
        close(lp[0]);
	close(lp[1]);
	lp = NULL;
	dup2(saved_stdin, 0);
      }
      
      dup2(fd, 0);
      ret = exec_cmd(p, NULL, NULL);
      close(fd);
      dup2(saved_stdin, 0);
      //close(saved_stdin);
      break;
    }
    else if(strcmp(link, "&&") == 0){
      ret = exec_cmd(p, NULL, NULL);

      if(lp != NULL){
        close(lp[0]);
	close(lp[1]);
	lp = NULL;
	dup2(saved_stdin, 0);
      }
      
      if(ret != 0){
        break;
      }
    }
    else if(strcmp(link, "||") == 0){
      ret = exec_cmd(p, NULL, NULL);

      if(lp != NULL){
        close(lp[0]);
	close(lp[1]);
	lp = NULL;
	dup2(saved_stdin, 0);
      }
      
      if(ret == 0){
        break;
      }
    }
    else if(strcmp(link, "|") == 0){
      if(p->next == NULL){
	fprintf(stderr, "-mpsh: erreur de syntaxe '|'\n");
        ret = -1;
	break;
      }
      int fd[2];
      pipe(fd);
      
      ret = exec_cmd(p, NULL, fd);
      
      if(lp != NULL){
        close(lp[0]);
	close(lp[1]);
	lp = NULL;
	dup2(saved_stdin, 0);
      }
      
      dup2(saved_stdout, 1);
      if(ret != 0){
        break;
      }
      //redirection entree de cmd 2
      lp = fd;
    }
    p = p->next;
  }

  if(lp != NULL){
    close(lp[1]);
    close(lp[0]);
    dup2(saved_stdin, 0);
  }
  
  return ret;
}

int exec_cmd(cmd* c, int* rp, int* wp){
  //printf("-%s-, -%s-, %i\n",c->args[0], c->args[1], c->nb_args);
  if(isVarCreationCmd(c->args[0])){
    return addNewVar(c->args[0]);
  }
  else if(isIntern(c->args[0])){
    return exec_intern(c, rp, wp);
  }
  else if(isExtern(c->args[0], PATH)){
    return exec_extern(c, rp, wp);
  }
  else{
    perror("-mpsh: command not found");
    return -1;
  }
}

int exec_intern(cmd* c, int* rp, int* wp){
  char* name = c->args[0];
  char** args = c->args;
  int nb_args = c->nb_args;

  if(rp != NULL){
    close(rp[1]);
    dup2(rp[0], 0);
    close(rp[0]);
  }
  if(wp != NULL){
    close(wp[0]);
    dup2(wp[1], 1);
    close(wp[1]);
  }
  
  if(strcmp(name, "alias") == 0){
    replaceAliasesInArgs(c);
    _alias(nb_args, args);
  }
  else if(strcmp(name, "unalias") == 0){
    _unalias(nb_args, args);
  }
  else if(strcmp(name, "type") == 0){
    _type(nb_args, args, PATH);
  }
  else if(strcmp(name, "history") == 0){
    replaceAliasesInArgs(c);
    char* reexec = NULL;
    int ret = _history(nb_args, args, &reexec);
    if(reexec != NULL){
      char* tmp = replaceAliasByValue(reexec);
      cmd* c2 = command(tmp);
      if(isIntern(c2->args[0])){
	ret = exec_intern(c2,NULL,NULL);
      }
      else if(isExtern(c2->args[0], PATH)){
	ret = exec_extern(c2,NULL, NULL);
      }
      else{
	      perror("-mpsh: command not found");
	      ret = 1;
      }
      free(tmp);
      free(c2);
      free(reexec);
    }
    return ret;
  }
  else if(strcmp(name, "umask") == 0){
    replaceAliasesInArgs(c);
    return _umask(nb_args, args);
  }
  else if(strcmp(name, "exit") == 0){
    char* tmp = malloc(100*sizeof(char));
    sprintf(tmp, "%i/env", getpid());
    remove(tmp);
    sprintf(tmp, "%i/vars", getpid());
    remove(tmp);
    sprintf(tmp, "%i/aliases", getpid());
    remove(tmp);

    sprintf(tmp, "%i", getpid());
    rmdir(tmp);
    
    free(tmp);
    replaceAliasesInArgs(c);
    return _exitcmd(nb_args, args, last_return);
  }
  else if(strcmp(name, "cd") == 0){
    replaceAliasesInArgs(c);
    return cd(nb_args, args);
  }
  else if(strcmp(name, "echo") == 0){
    replaceAliasesInArgs(c);
    return _echo(nb_args, args);
  }
  else if(strcmp(name, "export") == 0){
    replaceAliasesInArgs(c);
    return _export(nb_args, args);
  }
  else{
    perror("-mpsh: no such intern command");
    return 1;
  }
  return 0;
}

int exec_extern(cmd* c, int* rp, int* wp){
  char* name_exec=findExternCmd(c->args[0],PATH);

  if(name_exec == NULL){
    perror("No such extern command");
    return 1;
  }
  else{
    int pid;
    if((pid = fork())==0){
      if(rp != NULL){
	dup2(rp[0], 0);
	close(rp[1]);
	//close(rp[0]);
      }
      if(wp != NULL){
	dup2(wp[1], 1);
	close(wp[0]);
	//close(wp[1]);
      }
      execv(name_exec, c->args);
      exit(0);
    }
    else{
      int status;
      while(wait(&status)!=pid); // attendre la fin de ses fils
      free(name_exec);
      if (WIFEXITED(status)){
	return WEXITSTATUS(status);
      }
      else{
	return status;
      }
    }
  }
  return 0;
}


cmd *command(char * input){

  cmd *c=malloc(sizeof(cmd));
  char * cmd_name;
  char *args[MAX_ARGS];
  char *input_save = input;
  
  int i=1;
  while((cmd_name=strtok_r(input_save, " \n", &input_save))){
    args[i-1]=malloc(sizeof(cmd_name));
    strcpy(args[i-1],cmd_name);
    i++;
  }
  c->nb_args=i-1;

  c->args=malloc((c->nb_args+1)*sizeof(char *));
  for(int j=0; j<c->nb_args; j++){
    c->args[j]=malloc(strlen(args[j])*sizeof(char));
    strcpy(c->args[j],args[j]);
  }
  c->args[c->nb_args]=NULL;

  c->next = NULL;

  return c;
}
*/


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

