#ifndef __CLI__H__
#define __CLI__H__

#define BUF_LENGTH 1024
#define MAX_ARGS 10
#define NAME_MAX 256
#define DEF_INVITE "\033[01;32m /u @ /h : \033[01;34m /d \033[00m /$  >>> " //valeur par defaut de INVITE

/* la structure d'une commande */
typedef struct cmd{
    char nameCmd[NAME_MAX];
    char **args;
    int nb_args;
} cmd;

/* le format d'une commande */
typedef struct formatCmd{
    char name[NAME_MAX];
    int nbArgs;
    int requiredArgs; // arguments obligatoire ou non( 1 c'est obligatoire 0 sinon )
} formatCmd;

/* -------------------- Les prototypes ------------------- */
void initShell();

#endif