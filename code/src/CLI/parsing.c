#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CLI/cli.h"
#include "CLI/parsing.h"

cmd *CreateCmd(char *str) {
    char saveStr[BUF_LENGTH];
    strcpy(saveStr, str);

    cmd* cmd = constListCmds(saveStr);
    if( cmd != NULL){
        printf("\n  cmd = %s, nbArgs = %d , args : ", (cmd->args)[0], cmd->nb_args );
        if((cmd->nb_args) > 0) {
            int j;
            for(j = 1;j < (cmd->nb_args);j++){
                printf(" %s ", (cmd->args)[j]);
            }
        }
    }
    printf("\n");
    return cmd;
}


cmd* constListCmds(char *commande){

    cmd *c = malloc(sizeof(cmd));
    c->args = malloc(sizeof(char)*MAX_ARGS);
    parse(commande,c->args, &(c->nb_args), " ");
    
    return c;
}


int parse(char* str, char** strpiped, int *nbCmd, char* caracteres) 
{ 
    int i = 0;
    char* p = strtok(str, caracteres); 
    while (p != NULL) { 
        strpiped[i] = p; 
        i++;
        p = strtok(NULL, caracteres);
    }
    *nbCmd = i;
    if (strpiped[1] == NULL) 
        return 0; 
    else { 
        return 1; 
    } 
} 