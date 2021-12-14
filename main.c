#include "gamelib.h"
#include <stdio.h>

// Questo file continene solo la definizione della funzione main
// con il menu principale 1-2-3

int main( ) {
  //si dichiarano le varibili
  int flag=1;
  char scelta[256];
  int imp=0;
  printf("Benvenuti in questa piana desolata\n");
  //menu di scelta
  do{
    printf("\nCosa desiderate fare?\n1 : impostare il gioco\n2 : combattere!\n3 : terminare il gioco gioco\n\n");
    fgets(scelta, 256, stdin);

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
          printf("Il gioco deve essere impostato!\n");
        break;
      case 51:
        Termina_gioco();
        flag=0;
        break;
      default:
        printf("inserire solo 1, 2 o 3\n");
    }
  }while(flag);
}
