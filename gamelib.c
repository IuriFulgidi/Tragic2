#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

//variabili GLOBALI
static mago mago1;
static mago mago2;
static int ctrm=0;//contatore maghi
/*static carta* prima1=NULL;//prima e ultima carta dei mazzi
static carta* ultima1=NULL;
static carta* prima2=NULL;
static carta* ultima2=NULL;*/

//dichiarazione funzioni
static void Stampa_mago(mago);
static void Popola_mano(mago*);
static void Stampa_mano(mago*);
static void Stampa_carta(carta*);
static void Impila_mazzo(mago*, int);
static void Stampa_mazzo(mago);
static void Crea_mago(mago*);
static carta* Crea_carta(void);
static void Crea_campo(mago*);
static void Stampa_campo(mago*);

void Imposta_gioco(){
  system("clear");

  //variabili random
  time_t t;
  srand((unsigned)time(&t));

  //creazione dei maghi
  Crea_mago(&mago1);
  Crea_mago(&mago2);

  //numero di carte nel mazzo
  int n;
  int flag=0;
  do {
    printf("inserire il numero di carte dei mazzi, compreso tra 0 e 80\n");
    scanf("%d",&n);
    if(n<1 || n>80)
      flag=1;
    else
      flag=0;
  } while(flag);

  //stampa informazioni apperna messe
  Stampa_mago(mago1);
  Stampa_mago(mago2);

  //creazione delle mani
  Popola_mano(&mago1);
  Popola_mano(&mago2);

  Stampa_mano(&mago1);
  Stampa_mano(&mago2);

  //creazione mazzi
  Impila_mazzo(&mago1, n);
  Impila_mazzo(&mago2, n);

  Stampa_mazzo(mago1);
  Stampa_mazzo(mago2);

  Crea_campo(&mago1);
  Crea_campo(&mago2);

  Stampa_campo(&mago1);
  Stampa_campo(&mago2);

  printf("Gioco impostato!\n");

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

  m->PV=20;

  ctrm++;
  return;
}

static void Stampa_mago(mago m){
  printf("mago %s\n", m.nome );
  printf("classe: ");
  switch (m.classe) {
    case tenebre:
      printf("tenebre\n");
      break;
    case luce:
      printf("luce\n");
      break;
    case vita:
      printf("vita\n");
      break;
  }
  printf("punti vita: %d\n", m.PV );
  printf("\n");
}

static carta* Crea_carta(){
  carta *c = (carta*) malloc(sizeof(carta));
  int tipo =rand()%100;
  if(tipo<40){ //40% di probabilità che sia una creatura
    c->tipo=creatura;
  }
  else if(tipo<65){//25% di probabilità che sia un rimuovi creatura
    c->tipo=rimuovi_creatura;
  }
  else if(tipo<85){//20% di probabilità che sia un infliggi danno
    c->tipo=infliggi_danno;
  }
  else{//15% di probabilità che sia un guarisci danno
    c->tipo=guarisci_danno;
  }
  c->punti_vita= (rand()%6)+1;
  c->next= NULL;

  return c;
}

static void Popola_mano(mago *m){
  for (int i = 0; i < 5; i++) {
    carta *c =Crea_carta();
    m->mano[i]=c;
  }
  m->mano[5]=NULL;
  return;
}

static void Crea_campo(mago *m){
  for (int i = 0; i < 4; i++) {
    m->campo[i]=NULL;
  }
}

static void Stampa_campo(mago *m){
  printf("Campo del mago %s:\n", m->nome);
  for (int i = 0; i < 4; i++) {
    Stampa_carta(m->campo[i]);
  }
  printf("\n");
}

static void Stampa_mano(mago *m){
  printf("Mano del mago %s:\n", m->nome);
  for (int i = 0; i < 6; i++) {
    Stampa_carta(m->mano[i]);
  }
  printf("\n");
}

static void Stampa_carta(carta *c){
  if(c!=NULL){
    switch (c->tipo) {
      case creatura:
        printf("Creatura con %d punti vita\n", c->punti_vita);
        break;
      case rimuovi_creatura:
        printf("Rimuovi Creatura\n");
        break;
      case infliggi_danno:
        printf("Inflliggi %d danni \n", c->punti_vita);
        break;
      case guarisci_danno:
        printf("Guarisci %d danni\n", c->punti_vita);
        break;
      default:
        printf("Roba strana\n");
    }
  }
}

static void Impila_mazzo(mago* m, int n){
  for (int i = 0; i < n; i++) {
    carta *c =Crea_carta();
    if(m->inizio_mazzo == NULL){ // No card in the list
      m->inizio_mazzo=c;
    }
    else{
      carta *tmp = m->inizio_mazzo;
      while(tmp->next!=NULL){
        tmp = tmp->next;
      }
      tmp->next=c;
    }
  }
  return;
}

static void Stampa_mazzo(mago m){
  carta* prima = m.inizio_mazzo;
  printf("Mazzo:\n");
  if(prima == NULL){ // No node in the list
    printf("Mazzo vuoto!");
  }
  else{
    carta* pScan = prima;
    do{
      Stampa_carta(pScan);
      pScan = pScan->next;
    }while(pScan!= NULL);
  }
  printf("\n");
  return;
}

void Combatti(){
  printf("Combatti\n");
}

static void Pesca(mago* m){
  carta* ptemp= m->inizio_mazzo;
  if(ptemp==NULL)
    printf("Mazzo finito, fine\n");
  else if (ptemp->next == NULL){
    printf("Ultima carta del mazzo\n");
    //Inserisci_mano(m, ptemp);

  }
}

void Termina_gioco(){
  //free

  printf("Il gioco è terminato, attenderò in questa piana ventosa per il prossimo duello\n");
}

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
