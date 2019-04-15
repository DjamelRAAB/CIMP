#ifndef PARSING_H
#define PARSING_H

#include "CLI/cli.h"

extern int parse(char* str, char** strpiped, int *nbCmd,char* caracteres);
extern cmd* constListCmds(char *c);
extern cmd *CreateCmd(char *str);


#endif