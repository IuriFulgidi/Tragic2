#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>


//variabili
static mago mago1;
static mago mago2;
static int ctrm=0;//contatore maghi
static carta* prima1=NULL;//prima e ultima carta dei mazzi
static carta* ultima1=NULL;
static carta* prima2=NULL;
static carta* ultima2=NULL;

//dichiarazione funzioni
static carta* Popola_mano(carta*);
static void Stampa_mano(carta*);
static void Stampa_carta(carta);
static void Impila_mazzo(carta*, carta*);
static void Stampa_mazzo(carta*, int);
static void Crea_mago(mago*);

void Imposta_gioco(){
  system("clear");

  //variabili
  time_t t; //random
  srand((unsigned) time(&t));
  int flag=0;
  int n; //# carte del mazzo

  //creazione primo mago
  Crea_mago(&mago1);
  //creazione secondo mago
  Crea_mago(&mago2);

  //impostazione punnti vita
  mago1.PV=20;
  mago2.PV=20;

  //controllo numero di carte nel mazzo
  do {
    printf("inserire il numero di carte dei mazzi, compreso tra 0 e 80\n");
    scanf("%d",&n);
    if(n<1 || n>80)
      flag=1;
    else
      flag=0;
  } while(flag);

  //stampa informazioni apperna messe
  printf("Mago1\n");
  printf("nome: %s\n", mago1.nome );
  printf("classe: %d\n", mago1.classe );
  printf("punti vita: %d\n", mago1.PV );
  printf("\n");
  printf("Mago2\n");
  printf("nome: %s\n", mago2.nome );
  printf("classe: %d\n", mago2.classe );
  printf("punti vita: %d\n", mago2.PV );
  printf("\n");
  printf("ogni mazzo ha %d carte\n", n);


  //creazione della mano
  carta *mano1;
  mano1 = (carta*) malloc(6*sizeof(carta));
  mano1 = Popola_mano(mano1);
  mago1.mano=mano1;
  Stampa_mano(mano1);

  carta *mano2;
  mano2 = (carta*) malloc(6*sizeof(carta));
  mano2 = Popola_mano(mano2);
  mago2.mano=mano2;
  Stampa_mano(mano2);

  //creazione mazzi
  //primo mago
  carta *pNew = (carta*) malloc(sizeof(carta));
  pNew->tipo=rand()%4;
  pNew->punti_vita= (rand()%6)+1;
  pNew->next= NULL;
  prima1=pNew;
  for (int i = 0; i < n; i++) {
    Impila_mazzo(prima1, ultima1);
  }
  mago1.inzio_mazzo=prima1;
  mago1.fine_mazzo=ultima1;
  Stampa_mazzo(prima1, n);

  //secondo mago
  for (int i = 0; i < n; i++) {
    Impila_mazzo(prima2, ultima2);
  }
  mago2.fine_mazzo=prima2;
  mago2.inzio_mazzo=ultima2;
  Stampa_mazzo(mago2.inzio_mazzo, n);

}

static void Crea_mago(mago* m){
  int flag=0;
  char classe[256];

  if(ctrm%2==0)
    printf("Inserire il nome del primo mago\n");
  else
    printf("Inserire il nome del secondo mago\n");

  fgets(m->nome, 64, stdin);
  m->nome[strlen(m->nome)-1]='\0';

  if(ctrm%2==0)
    printf("Inserire la classe del primo mago\n");
  else
    printf("Inserire la classe del secondo mago\n");

  //spiegazione classi

  do {
    printf("Scegliere tra 'tenebre', 'vita' o 'luce'\n");
    fgets(classe, 256, stdin);
    for (int i = 0; i < strlen(classe); i++){
      classe [i]=tolower(classe[i]);
    }
    if (classe[0]=='t' && classe[1]=='e' && classe[2]=='n' && classe[3]=='e' && classe[4]=='b' && classe[5]=='r' && classe[6]=='e' && strlen(classe)==8){
      m->classe=tenebre;
      flag=0;
    }
    else if(classe[0]=='v' && classe[1]=='i' && classe[2]=='t' && classe[3]=='a' && strlen(classe)==5){
      m->classe=vita;
      flag=0;
    }
    else if(classe[0]=='l' && classe[1]=='u' && classe[2]=='c' && classe[3]=='e' && strlen(classe)==5){
      m->classe=luce;
      flag=0;
    }
    else
      flag=1;
  } while(flag);

  ctrm++;
  return;
}

static carta* Popola_mano(carta *c){
  for (int i = 0; i < 5; i++) {
    c[i].tipo=rand()%4;
    c[i].punti_vita= (rand()%6)+1;
    c[i].next=NULL;
  }
  c[5].punti_vita=0;
  return c;
}

static void Stampa_mano(carta *c){
  if(c==mago1.mano)
    printf("Mano del primo mago:\n");
  else
    printf("Mano del secondo mago:\n");
  for (int i = 0; i < 6; i++) {
    Stampa_carta(c[i]);
  }
}

static void Stampa_carta(carta c){
  if(c.punti_vita!=0){
    switch (c.tipo) {
      case creatura:
        printf("Creatura con %d punti vita\n", c.punti_vita);
        break;
      case rimuovi_creatura:
        printf("Rimuovi Creatura\n");
        break;
      case infliggi_danno:
        printf("Inflliggi %d danni \n", c.punti_vita);
        break;
      case guarisci_danno:
        printf("Guarisci %d danni\n", c.punti_vita);
        break;
      default:
        printf("Roba strana\n");
    }
  }
}

static void Impila_mazzo(carta* pr, carta* ul){
  // Creation of a new card in the heap
  carta *pNew = (carta*) malloc(sizeof(carta));
  pNew->tipo=rand()%4;
  pNew->punti_vita= (rand()%6)+1;
  pNew->next= NULL;

  if(pr == NULL){ // No card in the list
    pr = pNew; // The first card is the newly created one
    ul = pNew; // The last card is the newly created one
  }
  else{
    // Else, there is already at least one card in the list
    ul-> next= pNew; // the last card becomes the second one
    ul= pNew; // The last card is the newly created one
  }

  printf("puntatore prima carta%p\n", pr );
  printf("puntatore a questa carta%p\n", pNew );
  printf("puntatore alla prossima carta: %p\n", pNew->next );
  printf("puntatore ultima carta%p\n", ul );

  return;
}

static void Stampa_mazzo(carta* c, int n){
  printf("Mazzo:\n");
  for (int i = 0; i < n; i++) {
    Stampa_carta(c[i]);
  }
}


void Combatti(){
  printf("Combatti\n");
}

void Termina_gioco(){
  //free

  printf("Il gioco è terminato, attenderò in questa piana ventosa per il prossimo duello\n");
}

/*metodo fatto a lezione
static void stampa_mazzo(mago m){
  carta* ptemp = mago.inzio_mazzo;
  while(ptemp!=NULL){
    printf("tipo carta: %d\n", ptemp->tipo_carta );
    printf("%s\n");

    ptemp=ptemp->next;
  }
}*/


//print
/* As a parameter, it takes the pointer to the first node of a list
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
}*/

//tail insertion
/// Node* pFirst is a pointer to the first node and Node* pLast to the last node of
/* a list (both global)
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
}*/

/*tail deletion
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
}*/
