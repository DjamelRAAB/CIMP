#ifndef __CLI__H__
#define __CLI__H__

#define BUF_LENGTH 1024
#define MAX_ARGS 10

typedef struct cmd{
    char **args;
    int nb_args;
    //struct cmd *next;
} cmd;


#endif