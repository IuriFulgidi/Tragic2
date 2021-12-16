#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

//variabili GLOBALI
static mago mago1;
static mago mago2;
static mago* magoT;//mago di turn
static int ctrm=0;//contatore stampa maghi
static int n;//# carte dei mazzi
static int imp=1;//controlla che il gioco sia stato impostato
static int flag=1;//controlla menu combatti
static char scelta[256];//scelta menu combatti
static int flag_att=0;//flag metodi combatti
static int flag_gio=0;
static int flag_pes=0;
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
static carta* Crea_carta(mago*);
static void Crea_campo(mago*);
static void Stampa_campo(mago*);
static void Pesca(mago*);
static void Gioca(void);
static void Attacca(void);
static void Passa(void);

void Imposta_gioco(){
  system("clear");

  //variabili random
  time_t t;
  srand((unsigned)time(&t));

  Crea_mago(&mago1);
  Crea_mago(&mago2);

  //numero di carte nel mazzo
  int flag=0;
  do {
    printf("Inserire il numero di carte dei mazzi, compreso tra 0 e 80\n");
    scanf("%d",&n);
    if(n<1 || n>80)
      flag=1;
    else
      flag=0;
  } while(flag);

  Stampa_mago(mago1);
  Stampa_mago(mago2);

  Popola_mano(&mago1);
  Popola_mano(&mago2);

  Stampa_mano(&mago1);
  Stampa_mano(&mago2);

  Impila_mazzo(&mago1, n);
  Impila_mazzo(&mago2, n);

  Stampa_mazzo(mago1);
  Stampa_mazzo(mago2);

  Crea_campo(&mago1);
  Crea_campo(&mago2);

  Stampa_campo(&mago1);
  Stampa_campo(&mago2);

  imp=0;
  printf("Gioco impostato!\n");

}

static void Crea_mago(mago* m){
  //variabili
  int flag=0;
  char classe[256];

  //alternatore stampa
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

  printf("\nVita: le creature hanno la metà dei punti vita in più\n");
  printf("Tenebre: le carte infliggi danno, feriscono il doppio\n");
  printf("Luce: le carte guarisci danno, sono 3.5 volte più efficaci\n\n");

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
  printf("Mago %s\n", m.nome );
  printf("Classe: ");
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
  printf("Punti vita: %d\n", m.PV );
  printf("\n");
}

static carta* Crea_carta(mago* m){
  //creazione carta base
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

  //modifiche in base alla posizione nel mazzo
  c->punti_vita= (rand()%10)+1;
  //int max, min;
  //c->punti_vita= rand() % (max - min + 1) + min

  //modifiche n base alla classi del mago
  switch (m->classe){
    case vita:
      if(c->tipo==creatura)
        c->punti_vita=c->punti_vita*1.5;
      break;
    case tenebre:
      if(c->tipo==infliggi_danno)
        c->punti_vita=c->punti_vita*2;
      break;
    case luce:
      if(c->tipo==guarisci_danno)
        c->punti_vita=c->punti_vita*3.5;
      break;
  }

  c->next= NULL;
  return c;
}

static void Popola_mano(mago *m){
  for (int i = 0; i < 5; i++) {
    carta *c =Crea_carta(m);
    m->mano[i]=c;
  }
  m->mano[5]=NULL;//la sesta posizione è vuota
  return;
}

//si inizializzano le 4 zone del campo a null
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
        printf("Infliggi %d danni \n", c->punti_vita);
        break;
      case guarisci_danno:
        printf("Guarisci %d danni\n", c->punti_vita);
        break;
      default:
        printf("Houoston, abbiamo un problema\n");
    }
  }
}

static void Impila_mazzo(mago* m, int n){
  for (int i = 0; i < n; i++) {
    carta *c = Crea_carta(m); //nuova carta
    if(m->inizio_mazzo == NULL){ //Per la prima cart del mazzo
      m->inizio_mazzo=c;
    }
    else{
      carta *tmp = m->inizio_mazzo; //nuovo puntatore si salva la posizione della prima
      while(tmp->next!=NULL){//finché non siamo all'ultima carta
        tmp = tmp->next;//si passa alla successiva
      }
      tmp->next=c;//la nuova carta è l'ultima
    }
  }
  return;
}

static void Stampa_mazzo(mago m){
  carta* prima = m.inizio_mazzo;
    printf("Mazzo del mago %s:\n", m.nome);
  if(prima == NULL){ //Nel caso di lista vuota
    printf("Mazzo vuoto!");
  }
  else{
    carta* tmp = prima;
    do{
      Stampa_carta(tmp);//si stampa la carta
      tmp = tmp->next;//si passa alla successiva
    }while(tmp!= NULL);//finché non finisce la lista
  }
  printf("\n");
  return;
}


void Combatti(){
  system("clear");
  if(imp){
    printf("Il gioco deve prima essere impostato!\n");
    return;
  }

  if(rand()%2==0)
    magoT=&mago1;
  else
  magoT=&mago2;

  do{
    printf("Turno di %s\n",magoT->nome);
    printf("\nCosa desideri fare?\n1 : pescare una carta\n2 : giocare una carta\n3 : attaccare\n4 : stampare la tua mano\n5 : stampare il campo\n6 : passare il turno\n\n");
    fgets(scelta, 256, stdin);

    //si controlla che in input sia stato inserito un solo carattere
    if(scelta[1]!=10){
      printf("Inserire solo 1, 2, 3, 4, 5 o 6\n");
      flag=1;
      continue;
    }
    switch(scelta[0]){
      case 49:
        Pesca(magoT);
        break;
      case 50:
        Gioca();
        break;
      case 51:
        Attacca();
        break;
      case 52:
        Stampa_mano(magoT);
        break;
      case 53:
        Stampa_campo(&mago1);
        Stampa_campo(&mago2);
        break;
      case 54:
        Passa();
        break;
      default:
        printf("inserire solo 1, 2, 3, 4, 5 o 6\n");
    }
  }while(flag);
}



static void Pesca(mago* m){
  if(flag_pes){
    printf("Puoi giocare una sola carta per turno!\n");
    return;
  }

  carta* tmp= m->inizio_mazzo;
  if(tmp==NULL)
    printf("Mazzo finito, fine\n");
  else if (tmp->next == NULL){
    printf("Ultima carta del mazzo\n");
    //Inserisci_mano(m, ptemp);

  }

  flag_pes=1;
}

static void Gioca(){
  if(flag_gio){
    printf("Puoi giocare una sola carta per turno!\n");
    return;
  }

  printf("Gioca\n");

  flag_gio=1;
}

static void Attacca(){
  if(flag_att){
    printf("Puoi attaccare una sola volta per turno!\n");
    return;
  }

  printf("Attacca\n");

  flag_att=1;
}

static void Passa(){
  if(magoT==&mago1)
    magoT=&mago2;
  else
    magoT=&mago1;

  //controllo se uno è morto??
}

void Termina_gioco(){
  system("clear");
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
