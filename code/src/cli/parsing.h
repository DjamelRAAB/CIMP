#ifndef PARSING_H
#define PARSING_H

#include "cli.h"

extern int parse(char* str, char *nameCmd ,char** argsCmd, int *nbCmd,char* caracteres);
extern cmd* constListCmds(char *c);
extern cmd *CreateCmd(char *str);


#endif