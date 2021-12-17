#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

//variabili globali
static mago mago1;
static mago mago2;
static mago* magoT;//mago di turn
static int ctrm=0;//contatore stampa maghi
static int n;//# carte dei mazzi
static int imp=1;//controlla che il gioco sia stato impostato
static short flag_mc=1;//controlla menu combatti
static char scelta[256];//scelta menu combatti
static short flag_att=0;//flag metodi combatti
static short flag_gio=0;
static short flag_pes=0;
/*static carta* prima1=NULL;//prima e ultima carta dei mazzi
static carta* ultima1=NULL;
static carta* prima2=NULL;
static carta* ultima2=NULL;*/

//dichiarazione funzioni
static void Crea_mago(mago*);
static void Stampa_mago(mago);
static carta* Crea_carta(mago*, int);
static void Stampa_carta(carta*);
static void Crea_mano(mago*);
static void Stampa_mano(mago*);
static void Crea_campo(mago*);
static void Stampa_campo(mago*);
static void Impila_mazzo(mago*);
static void Stampa_mazzo(mago);
static void Pesca(mago*);
static void Aggiungi_mano(mago*, carta*);
static void Gioca(void);
static void Attacca(void);
static void Passa(void);
static void Termina_partita();

void Imposta_gioco(){
  system("clear");

  //variabili random
  time_t t;
  srand((unsigned)time(&t));

  Crea_mago(&mago1);
  Crea_mago(&mago2);

  //numero di carte nel mazzo
  short flag=0;
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

  Crea_mano(&mago1);
  Crea_mano(&mago2);

  Stampa_mano(&mago1);
  Stampa_mano(&mago2);

  Impila_mazzo(&mago1);
  Impila_mazzo(&mago2);

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
  short flag=0;
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
  printf("Punti vita: %.1f\n", m.PV );
  printf("\n");
}

static carta* Crea_carta(mago* m, int pos){
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
  int max, min;
  double f = n/3;

  if((double)pos>2*f){
    min=1;
    max=4;
  }
  else if((double)pos>f){
    min=4;
    max=7;
  }
  else{
    min=7;
    max=10;
  }

  c->punti_vita= (rand()%(max-min+1))+min;

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

static void Stampa_carta(carta *c){
  if(c!=NULL){
    switch (c->tipo) {
      case creatura:
        printf("Creatura con %.1f punti vita\n", c->punti_vita);
        break;
      case rimuovi_creatura:
        printf("Rimuovi Creatura\n");
        break;
      case infliggi_danno:
        printf("Infliggi %.1f danni \n", c->punti_vita);
        break;
      case guarisci_danno:
        printf("Guarisci %.1f danni\n", c->punti_vita);
        break;
      default:
        printf("Houston, abbiamo un problema\n");
    }
  }
}

static void Crea_mano(mago *m){
  for (int i = 0; i < 5; i++) {
    carta *c =Crea_carta(m, n);
    m->mano[i]=c;
  }
  m->mano[5]=NULL;//la sesta posizione è vuota
  return;
}

static void Stampa_mano(mago *m){
  printf("Mano del mago %s:\n", m->nome);
  for (int i = 0; i < 6; i++) {
    Stampa_carta(m->mano[i]);
  }
  printf("\n");
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

static void Impila_mazzo(mago* m){
  for (int i = 0; i < n; i++) {
    carta *c = Crea_carta(m, i); //nuova carta
    if(m->inizio_mazzo == NULL){ //Per la prima cart del mazzo
      m->inizio_mazzo=c;
      m->fine_mazzo=c;
    }
    else{
      m->fine_mazzo->next=c;
      m->fine_mazzo=c;
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

  printf("\nTurno di %s\n",magoT->nome);

  do{
    printf("\nCosa desideri fare?\n1 : pescare una carta\n2 : giocare una carta\n3 : attaccare\n4 : stampare la tua mano\n5 : stampare il campo\n6 : passare il turno\n\n");
    fgets(scelta, 256, stdin);

    //si controlla che in input sia stato inserito un solo carattere
    if(scelta[1]!=10){
      printf("Inserire solo 1, 2, 3, 4, 5 o 6\n");
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
  }while(flag_mc);
}

static void Pesca(mago* m){
  if(flag_pes){
    printf("Puoi pescare una sola volta per turno!\n");
    return;
  }

  carta* tmp= m->inizio_mazzo;
  if(tmp==NULL){
    printf("Mazzo finito\n");
    Termina_partita();
  }
  else if (tmp->next == NULL){//se non c'è una prossima carta
    printf("Attenzione, ultima carta del mazzo\n");
    Aggiungi_mano(m, tmp);//si pesca questa carta
    m->inizio_mazzo=NULL;//non ci sono altre carte del mazzo
  }
  else{
    while((tmp->next)->next!=NULL)//finché ci sono carte
      tmp=tmp->next;//si scorre fino alla penultima carta
    Aggiungi_mano(m, tmp->next);//si aggiunge l'ultima carta
    tmp->next=NULL;//si toglie la carta pescata dal mazzo
  }

  flag_pes=1;
}

static void Aggiungi_mano(mago* m, carta* c){
  short flag=1;
  for (int i = 0; i < 6; i++){
    if(m->mano[i]==NULL){//se la posizione nella manoo è libera
      m->mano[i]=c;
      printf("Hai pescato una carta ");
      Stampa_carta(c);
      flag=0;
    }
  }
  if(flag){// se non ci sono posizioni libere
    printf("Mano piena, la carta viene distrutta\n");
    free(c);
  }
}

static void Gioca(){
  if(flag_gio){
    printf("Puoi giocare una sola carta per turno!\n");
    return;
  }

  //variabili
  int s;//per la scelta della carta
  short flag=0;//controllo scelta

  Stampa_mano(magoT);

  do{
    printf("\ninserire un numero da 1 a 6 per scegliere la carta che vuoi giocare\n");
    scanf("%d",&s);

    //controllo
    if(s<1 || s>6){
      printf("Il numero deve essere compreso tra 1 e 6\n");
      flag=1;
    }
    else{
      flag=0;

      carta* c=magoT->mano[s-1];//carta che viene giocata

      switch (c->tipo) {
        case creatura:{
          short flag_c=1;
          for (int i = 0; i < 4; i++){
            if(magoT->campo[i]==NULL){//la prima posizione libera nella campo
              magoT->campo[i]=c;//si mette la creatura sul campo
              printf("Hai posizionato sul campo la ");
              Stampa_carta(c);
              flag_c=0;
              break;
            }
          }
          if(flag_c){// se non ci sono posizioni libere
            printf("Campo pieno, non puoi giocare una creatura\n");
          }
          break;
        }
        case rimuovi_creatura:{
          //si prende il mago avversario a quello di turno
          mago* mago_avv=&mago1;
          if(magoT==&mago1)
            mago_avv=&mago2;

          //si controlla che ci sia almeno una creatura da rimuovere
          short flag_r=1;
          int ctr_r=0;
          for (int i = 0; i < 4; i++) {
            if(mago_avv->campo[i]!=NULL){
              flag_r=0;
              ctr_r++;
            }
          }
          if(flag_r){
            printf("Non ci sono creature avversarie da rimuovere!\n");
            break;
          }

          //si rimuove una creatura a scelta del mago
          flag_r=0;
          do{
            printf("inserire un numero tra 1 e %d per scegliere la creatura avversaria da rimuovere\n",ctr_r);
            int p;
            scanf("%d",&p );

            if(p<1||p>ctr_r)
              flag_r=1;
            else{
              free(mago_avv->campo[p-1]);
              mago_avv->campo[p-1]=NULL;
              printf("Creatura rimossa\n");
              flag_r=0;
            }

          }while (flag_r);
          break;
        }
        case infliggi_danno:
          //si sceglie il bersaglio da danneggiare
          break;
        case guarisci_danno:
          //si sceglie il bersaglio da guarire
          break;
      }
    }
    magoT->mano[s-1]=NULL;//la carta giocata non è più nella mano del mago
    //si sistema la mano
    for (int i = 0; i < 5; i++) {
      if(magoT->mano[i]==NULL && magoT->mano[i+1]!=NULL){//se c'è un buco nella mano
        magoT->mano[i]=magoT->mano[i+1];//si fanno scorrere il buco fino in fondo alla mano
        magoT->mano[i+1]=NULL;
      }
    }
  }while(flag);



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
  //cambio mago di turno
  if(magoT==&mago1)
    magoT=&mago2;
  else
    magoT=&mago1;

  printf("\nTurno di %s\n",magoT->nome);

  //abbasso le flag
  flag_pes=0;
  flag_gio=0;
  flag_att=0;

}

static void Termina_partita(){
  flag_mc=0;
  printf("\nPartita finita\n");
  if(mago1.PV>mago2.PV){
    printf("Ha vinto %s!\n",mago1.nome );
    printf("%s è condannato a diventare una cavia per gli esperimenti della Lega Izzet\n", mago2.nome);
  }
  else if(mago1.PV<mago2.PV){
    printf("Ha vinto %s!\n",mago2.nome );
    printf("%s è condannato a diventare una cavia per gli esperimenti della Lega Izzet\n", mago1.nome);
  }
  else{
    printf("Pareggio\n");
  }
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
