#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( ) {
  //si dichiarano le varibili
  short continua=1;//controlla inserimento 1 2 3 4
  short stampa_info=1;//evita duplicazioni di stampa
  char scelta[256];

  clear_screen();
  printf("Benvenuti in questa piana desolata\n");
  //menu di scelta
  do{
    if(stampa_info)
      printf("\nCosa desiderate fare?\n1 : impostare il gioco\n2 : combattere!\n3 : visualizzare le regole\n4 : terminare il gioco\n\n");

    if(fgets(scelta, sizeof(scelta), stdin)==NULL){
      perror("errore di sistema\n");
      return -1;
    }

    if(strlen(scelta)>1){
      stampa_info=1;
      //si controlla che in input sia stato inserito un solo carattere
      if(scelta[1]!=10){
        printf("Inserire solo 1, 2, 3 o 4\n");
        continua=1;
        continue;
      }
      switch(scelta[0]){
        case '1':
          Imposta_gioco();
          break;
        case '2':
          Combatti();
          break;
        case '3':
          Regole();
          break;
        case '4':
          Termina_gioco();
          continua=0;
          break;
        default:
          printf("inserire solo 1, 2, 3 o 4\n");
      }
    }
    else
      stampa_info=0;
  }while(continua);
}
