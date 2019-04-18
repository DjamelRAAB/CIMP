#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include "cli.h"
#include "parsing.h"


/* Au lancement du Shell  */
void initShell(){
  printf("\033[H\033[J");
  printf("\n\n\n\n\t\t******************************************\n"); 
  printf("\n\t\t*************** CIMP *********************\n") ;
  printf("\n\t\t******************************************\n\n"); 
  //sleep(1); 
  printf("\033[H\033[J");
}



