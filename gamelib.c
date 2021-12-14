#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//prima e ultima carta del mazzo
carta* prima=NULL;
carta* ultima=NULL;

//print
// As a parameter, it takes the pointer to the first node of a list
static void print_list(carta* prima){
  if(prima == NULL){ // No node in the list
    printf("No node in the list!");
  }
  else{
    // New pointer used to scan the list.
    carta* pScan = prima;
    do{
      printf("Info: %d\n", pScan->punti_vita);
      // ptrScan is updated to point to the next node in the
      // list
      pScan = pScan->next;
    }while(pScan!= NULL); //NULL when this was the last node
  }
  return;
}

//tail insertion
/// Node* pFirst is a pointer to the first node and Node* pLast to the last node of
// a list (both global)
static void tail_insertion(void){
  // Creation of a new node in the heap
  carta *pNew = (carta*) malloc(sizeof(carta));
  scanf("%d", &(pNew->punti_vita));
  pNew->next= NULL;
  if(prima == NULL){ // No node in the list
    prima = pNew; // The first node is the newly created one
    ultima = pNew; // The last node is the newly created one
  }
  else{
    // Else, there is already at least one node in the list
    ultima-> next= pNew; // the last node becomes the second one
    ultima= pNew; // The last node is the newly created one
  }
  return;
}

//tail deletion
static void tail_deletion() {
  if(prima == NULL)
    printf("No node in the list!\n");
  else {
    carta* pPrev = NULL;
    carta* pScan = prima;
    if(pScan->next == NULL) {// It means we only have one node in the list
      free(pScan); // Free memory
      prima= NULL; // Now the list is empty
    }
    else {// Otherwise, I need to scan the list until I find the last node (pLast)
      do{
        if((pScan-> next) == ultima) {// Reached the node before the end
          pPrev = pScan;
          break;
        }
        else
          pScan= pScan-> next; // Otherwise, I need to iterate
      }while((pScan-> next) != NULL);

      free(pPrev-> next); // Free memory allocated to the last node
      pPrev-> next = NULL; // pPrev becomes the last node (no node after it)
      ultima = pPrev; // pPrev becomes the last node
    }
  }
}

void Imposta_gioco(){
  //si dhichiarano variabili
  int flag=0;
  char classe[256];
  int n;

  //definizione maghi
  mago mago1;
  mago mago2;

  printf("Inserire il nome del primo mago\n");
  fgets(mago1.nome, 256, stdin);
  mago1.nome[strlen(mago1.nome)-1]='\0';
  printf("inserire la classe del primo mago\n");

  //spiegazione classi

  do {
    printf("Scegliere tra 'tenebre', 'vita' o 'luce'\n");
    fgets(classe, 256, stdin);
    for (int i = 0; i < strlen(classe); i++){
      classe [i]=tolower(classe[i]);
    }
    if (classe[0]=='t' && classe[1]=='e' && classe[2]=='n' && classe[3]=='e' && classe[4]=='b' && classe[5]=='r' && classe[6]=='e'){
      mago1.classe=tenebre;
      flag=0;
    }
    else if(classe[0]=='v' && classe[1]=='i' && classe[2]=='t' && classe[3]=='a'){
      mago1.classe=vita;
      flag=0;
    }
    else if(classe[0]=='l' && classe[1]=='u' && classe[2]=='c' && classe[3]=='e'){
      mago1.classe=luce;
      flag=0;
    }
    else
      flag=1;
  } while(flag);

  printf("Inserire il nome del secondo mago\n");
  fgets(mago2.nome, 256, stdin);
  mago2.nome[strlen(mago2.nome)-1]='\0';

  printf("inserire la classe del secondo mago\n");
  do {
    printf("Scegliere tra 'tenebre', 'vita' o 'luce'\n");
    fgets(classe, 256, stdin);
    for (int i = 0; i < strlen(classe); i++){
      tolower(classe[i]);
    }

    if (classe[0]=='t' && classe[1]=='e' && classe[2]=='n' && classe[3]=='e' && classe[4]=='b' && classe[5]=='r' && classe[6]=='e'){
      mago2.classe=tenebre;
      flag=0;
    }
    else if(classe[0]=='v' && classe[1]=='i' && classe[2]=='t' && classe[3]=='a'){
      mago2.classe=vita;
      flag=0;
    }
    else if(classe[0]=='l' && classe[1]=='u' && classe[2]=='c' && classe[3]=='e'){
      mago2.classe=luce;
      flag=0;
    }
    else
      flag=1;
  } while(flag);

  mago1.PV=20;
  mago2.PV=20;

  do {
    printf("inserire il numero di carte dei mazzi, compreso tra 20 e 80\n");
    scanf("%d",&n);
    if(n<20 || n>80)
      flag=1;
    else
      flag=0;
  } while(flag);

  printf("Mago1\n");
  printf("nome: %s\n", mago1.nome );
  printf("classe: %d\n", mago1.classe );
  printf("punti vita: %d\n", mago1.PV );
  printf("\n");
  printf("Mago2\n");
  printf("nome: %s\n", mago2.nome );
  printf("classe: %d\n", mago2.classe );
  printf("punti vita: %d\n", mago2.PV );



}

void Combatti(){
  printf("Combatti\n");
}

void Termina_gioco(){
  //free

  printf("Il gioco è terminato, attenderò in questa piana ventosa per il prossimo duello\n");
}
