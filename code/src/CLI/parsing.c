#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cli.h"
#include "parsing.h"

cmd *CreateCmd(char *str) {
    char saveStr[BUF_LENGTH];
    strcpy(saveStr, str);

    cmd* c = constListCmds(saveStr);
    if( c != NULL){
        printf("\n Parssing: cmd = %s , nbArgs = %d , args : ", c->nameCmd, c->nb_args );
        if((c->nb_args) > 0) {
            int j;
            for(j = 0;j < (c->nb_args);j++){
                printf(" %s ", (c->args)[j]);
            }
        }
    }
    printf("\n");
    return c;
}


cmd* constListCmds(char *commande){

    cmd *c = malloc(sizeof(cmd));
    c->args = malloc(sizeof(char)*MAX_ARGS);
    parse(commande,c->nameCmd,c->args, &(c->nb_args), " ");
    
    return c;
}


int parse(char* str, char *nameC ,char** argsCmd, int *nbCmd, char* caracteres) 
{ 
    int i = 0;
    char* p = strtok(str, caracteres); 
    /* Récuperer le nom de la commande */
    strcpy(nameC, p);

    /* Récuperer les arguments */
    p = strtok(NULL, caracteres);
    while (p != NULL) { 
        argsCmd[i] = p; 
        i++;
        p = strtok(NULL, caracteres);
    }
    *nbCmd = i;

    if (argsCmd[0] == NULL) 
        return 0; 
    else { 
        return 1; 
    } 
} 