#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( ) {
  //si dichiarano le varibili
  int flag=1;//controlla inserimento 1 2 3
  int flagp=0;//evita duplicazioni di stampa
  char scelta[256];
  int imp=0;

  system("clear");
  printf("Benvenuti in questa piana desolata\n");
  //menu di scelta
  do{
    if(!flagp)
      printf("\nCosa desiderate fare?\n1 : impostare il gioco\n2 : combattere!\n3 : terminare il gioco gioco\n\n");
    fgets(scelta, 256, stdin);

    if(strlen(scelta)>1){
      flagp=0;
      //si controlla che in input sia stato inserito un solo carattere
      if(scelta[1]!=10){
        printf("Inserire solo 1,2 o 3\n");
        flag=1;
        continue;
      }
      switch(scelta[0]){
        case 49:
          Imposta_gioco();
          imp=1;
          break;
        case 50:
          if(imp)
            Combatti();
          else
            printf("Il gioco deve prima essere impostato!\n");
          break;
        case 51:
          Termina_gioco();
          flag=0;
          break;
        default:
          printf("inserire solo 1, 2 o 3\n");
      }
    }
    else
      flagp=1;
  }while(flag);
}
