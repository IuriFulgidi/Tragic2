#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( ) {
  //si dichiarano le varibili
  short flag=1;//controlla inserimento 1 2 3 4
  short flagp=0;//evita duplicazioni di stampa
  char scelta[256];

  system("clear");
  printf("Benvenuti in questa piana desolata\n");
  //menu di scelta
  do{
    if(!flagp)
      printf("\nCosa desiderate fare?\n1 : impostare il gioco\n2 : combattere!\n3 : visualizzare le regole\n4 : terminare il gioco\n\n");
    fgets(scelta, 256, stdin);

    if(strlen(scelta)>1){
      flagp=0;
      //si controlla che in input sia stato inserito un solo carattere
      if(scelta[1]!=10){
        printf("Inserire solo 1, 2, 3 o 4\n");
        flag=1;
        continue;
      }
      switch(scelta[0]){
        case 49:
          Imposta_gioco();
          break;
        case 50:
            Combatti();
          break;
        case 51:
            Regole();
          break;
        case 52:
          Termina_gioco();
          flag=0;
          break;
        default:
          printf("inserire solo 1, 2, 3 o 4\n");
      }
    }
    else
      flagp=1;
  }while(flag);
}
