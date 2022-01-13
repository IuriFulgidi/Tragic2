#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

//variabili globali
static mago mago1;
static mago mago2;
static mago* mago_di_turno;
static int ctr_m=0;//contatore stampa maghi
static int n;//# carte dei mazzi
static short impostato=0;//controlla che il gioco sia stato impostato
static short partita_terminata=0;//controlla menu combatti
static char scelta[256];//scelta menu combatti
static short attaccato=0;//flag per le funzioni in combatti
static short giocato=0;
static short pescato=0;

//dichiarazione funzioni
//per Imposa_gioco
static void Crea_mago(mago*);
static void Stampa_mago(mago*);
static carta* Crea_carta(mago*, int);
static void Stampa_carta(carta*);
static void Crea_mano(mago*);
static void Stampa_mano(mago*);
static void Crea_campo(mago*);
static void Stampa_campo(mago*);
static void Impila_mazzo(mago*);
//per Gioca
static void Stampa_mago_turno(void);//stampa il nome del mago di turno
static void Stampa_PV(mago*);
static void Pesca(mago*);
static void Aggiungi_mano(mago*, carta*);//aggiunga alla mano di questo mago, questa carta
static void Gioca(void);
static void Attacca(void);
static void Sistema_campo(mago*);//riordina il campo quando una creatura viene rimossa
static void Passa(void);
static void Termina_partita(void);
//funzioni di utilità
static void Dealloca(void);//per liberare la memoria
static void Dealloca_mano(mago*);
static void Dealloca_mazzo(mago*);
static void Dealloca_campo(mago*);
static int Inserisci_numero(void);//per controllora inserimenti strani alla richiesta di un numero

void Imposta_gioco(){
system("clear");

  //se il giocco è stato già impostato si llbera la memoria precedente
  if(impostato)
    Dealloca();

  //variabili random
  time_t t;
  srand((unsigned)time(&t));

  Crea_mago(&mago1);
  Crea_mago(&mago2);

  //numero di carte nel mazzo
  short flag_tmp=0;
  do {
    char tmp[256];//stringa di controllo
    printf("Inserire il numero di carte dei mazzi, tra 0 e 80, non compresi\n");
    fgets(tmp, 256, stdin);
    if(strlen(tmp)>1){//per evitare doppie stampe
      flag_tmp=0;
      if(strlen(tmp)>3){  //si controlla che in input sia stato inserito solo 1 o 2 caratteri
        flag_tmp=1;
        continue;
      }
      if(strlen(tmp)==2){//se è stata inserita una stringa con un carattere
        if(tmp[0]>48 && tmp[0]<58){//si controlla che sia un numero
          n=((int)tmp[0])-48;
          flag_tmp=0;
        }
        else
          flag_tmp=1;
      }
      else{// se è stato inserito una stringa con due caratteri
        if(tmp[0]>47 && tmp[0]<56 && tmp[1]>47 && tmp[1]<58){//si controlla che la cifra delle decine sia tra 0 e 7, e che quella delle unità sia un numero
          int decine = (((int)tmp[0])-48)*10;
          int unita = ((int)tmp[1])-48;
          n= decine+unita;
          flag_tmp=0;
        }
        else
          flag_tmp=1;

        if(n==0)//nel caso in cui l'utente abbia inserito 00
          flag_tmp=1;
      }
    }
    else
      flag_tmp=1;
  } while(flag_tmp);

  //si popolano tutte le strutture dati del gioco
  Crea_mano(&mago1);
  Crea_mano(&mago2);
  Impila_mazzo(&mago1);
  Impila_mazzo(&mago2);
  Crea_campo(&mago1);
  Crea_campo(&mago2);

  //riepilogo delle informazioni impostate
  system("clear");
  printf("Gioco impostato!\n\n");
  Stampa_mago(&mago1);
  Stampa_mago(&mago2);
  printf("Mazzi di %d carte\n",n );

  impostato=1;
  partita_terminata=0;//per due partite consecutive
  return;
}

static void Crea_mago(mago* m){
  //variabili
  short flag=0;
  char classe[256];

  //alternatore stampa
  if(ctr_m%2==0)
    printf("Inserire il nome del primo mago\n");
  else
    printf("Inserire il nome del secondo mago\n");

  fgets(m->nome, 64, stdin);
  m->nome[strlen(m->nome)-1]='\0';//evita ritorni a capo indesiderati

  if(ctr_m%2==0)
    printf("\nInserire la classe del primo mago\n");
  else
    printf("\nInserire la classe del secondo mago\n");

  printf("\nVita: le \e[0;32mcreature\e[0m hanno la metà dei punti vita in più\n");
  printf("Tenebre: le carte \e[0;31minfliggi danno\e[0m, feriscono il doppio\n");
  printf("Luce: le carte \e[0;34mguarisci danno\e[0m, sono 3.5 volte più efficaci\n\n");

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

  ctr_m++;
  return;
}

static void Stampa_mago(mago* m){
  printf("Mago %s\n", m->nome );
  printf("Classe: ");
  switch (m->classe) {
    case tenebre:
      printf("tenebre\n");
      break;
    case luce:
      printf("luce\n");
      break;
    case vita:
      printf("vita\n");
      break;
    default:
      printf("Il Frazzi è un bel ragazzo\n");
  }
  printf("Punti vita: %.1f\n\n", m->PV );
  return;
}

static carta* Crea_carta(mago* m, int pos){
  //creazione carta base
  carta *c = (carta*) malloc(sizeof(carta));

  //tipo
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

  //punti vita
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

  //modifiche in base alla classe del mago
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
    default:
      printf("00101010\n");
  }

  //Nome
  switch (c->tipo) {
    case creatura:{
      int n_nomi=12;//da cambiare all'aggiunta di nuovi nomi
      char nomi[12][40]={
        "Tarmogoyf",
        "Morophon l'Illuminato",
        "Ovar l'Innimorfo",
        "Arcangelo di Thune",
        "Thrun l'ultimo Troll",
        "Batterskull",
        "Mago SnapCaster",
        "Liliana del Velo",
        "Nemesi del Vero Nome",
        "Bestia Procacciatrice",
        "Nethoroi Alfa della Morte",
        "Necropantera",
      };
      strcpy(c->nome, nomi[rand()%n_nomi]);
      break;
    }
    case rimuovi_creatura:{
      int n_nomi=4;//da cambiare all'aggiunta di nuovi nomi
      char nomi[4][40]={
        "Pulizia Planare",
        "Spade ai Vomeri",
        "Profezia di Fuoco",
        "Rubapensieri",
      };
      strcpy(c->nome, nomi[rand()%n_nomi]);
      break;
    }
    case infliggi_danno:{
      int n_nomi=7;//da cambiare all'aggiunta di nuovi nomi
      char nomi[7][40]={
        "Tempesta Cerebrale",
        "Maledizione Della Stregoneria",
        "Dipartita Dorata",
        "Fulmine",
        "Tifone di Squali",
        "Risucchia Vita",
        "Bacio della Vipera",
      };
      strcpy(c->nome, nomi[rand()%n_nomi]);
      break;
    }
    case guarisci_danno:{
      int n_nomi=6;//da cambiare all'aggiunta di nuovi nomi
      char nomi[6][40]={
        "Forza di Volonta",
        "Benedizione della Cappellana",
        "Anello Solare",
        "Benedizione Benevola",
        "Benedizione del Gelo",
        "Loto Nero",
      };
      strcpy(c->nome, nomi[rand()%n_nomi]);
      break;
    }
    default:
      printf("Viva YuGiOh!\n");
  }

  c->next= NULL;
  return c;
}

static void Stampa_carta(carta *c){
  if(c!=NULL){
    switch (c->tipo) {
      case creatura:
        printf("\e[0;32m%s\e[0m, creatura con %.1f punti vita\n",c->nome, c->punti_vita);
        break;
      case rimuovi_creatura:
        printf("\e[0;35m%s\e[0m, rimuovi creatura\n",c->nome);
        break;
      case infliggi_danno:
        printf("\e[0;31m%s\e[0m, infliggi %.1f danni \n",c->nome, c->punti_vita);
        break;
      case guarisci_danno:
        printf("\e[0;34m%s\e[0m, guarisci %.1f danni\n",c->nome, c->punti_vita);
        break;
      default:
        printf("Houston, abbiamo un problema\n");
    }
  }
  return;
}

static void Crea_mano(mago *m){
  for (int i = 0; i < 5; i++) {
    carta *c =Crea_carta(m, n);
    m->mano[i]=c;
  }
  m->mano[5]=NULL;//la sesta posizione è sempre vuota
  return;
}

static void Stampa_mano(mago *m){
  printf("Mano del mago %s:\n", m->nome);
  for (int i = 0; i < 6; i++) {
    if(m->mano[i]!=NULL)
      printf("%d : ", i+1);
    Stampa_carta(m->mano[i]);
  }
  printf("\n");
  return;
}

//si inizializzano le 4 zone del campo a null
static void Crea_campo(mago *m){
  for (int i = 0; i < 4; i++) {
    m->campo[i]=NULL;
  }
  return;
}

static void Stampa_campo(mago *m){
  printf("Campo del mago %s:\n", m->nome);
  for (int i = 0; i < 4; i++) {
    if(m->campo[i]!=NULL)
      printf("%d : ", i+1);
    Stampa_carta(m->campo[i]);
  }
  printf("\n");
  return;
}

static void Impila_mazzo(mago* m){
  for (int i = 0; i < n; i++) {
    carta *c = Crea_carta(m, i); //nuova carta
    if(m->inizio_mazzo == NULL){ //Per la prima carta del mazzo
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

void Combatti(){
  //abbasso le flag nel caso vengano fatte due partite di seguito
  pescato=0;
  giocato=0;
  attaccato=0;

  short flagp=0;//evita duplicazioni di stampa
  system("clear");
  if(!impostato){
    printf("Il gioco deve essere impostato almeno una volta!\n");
    return;
  }
  if(partita_terminata){
    printf("Il gioco deve essere impostato nuovamente al termine di una partita!\n");
    return;
  }

  if(rand()%2==0)
    mago_di_turno=&mago1;
  else
    mago_di_turno=&mago2;

  Stampa_mago_turno();
  do{
    if(!flagp)
      printf("\nCosa desideri fare?\n1 : pescare una carta\n2 : giocare una carta\n3 : attaccare\n4 : stampare la tua mano\n5 : stampare il campo\n6 : passare il turno\n\n");
    fgets(scelta, 256, stdin);

    //si controlla che in input sia stato inserito un solo carattere
    if(strlen(scelta)>1){
      flagp=0;
      if(scelta[1]!=10){
        printf("Inserire solo 1, 2, 3, 4, 5 o 6\n");
        continue;
      }
      switch(scelta[0]){
        case 49:
          Pesca(mago_di_turno);
          break;
        case 50:
          Gioca();
          break;
        case 51:
          Attacca();
          break;
        case 52:
          Stampa_mano(mago_di_turno);
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
    }
    else
      flagp=1;
  }while(!partita_terminata);
}

static void Stampa_mago_turno(){
  switch (mago_di_turno->classe) {
    case vita:
      printf("\nTurno del vivace %s\n",mago_di_turno->nome);
      break;
    case tenebre:
      printf("\nTurno del tenebroso %s\n",mago_di_turno->nome);
      break;
    case luce:
      printf("\nTurno dello splendente %s\n",mago_di_turno->nome);
      break;
    default:
      printf("Secondo la Bibbia è nata prima la gallina\n");
  }
}

static void Stampa_PV(mago *m){
  if(m->PV<0)//per evitare stampe brutte
    m->PV=0;
  printf("Ora %s ha %.1f punti vita\n", m->nome, m->PV);
}

static void Pesca(mago* m){
  if(pescato){
    printf("Puoi pescare una sola volta per turno!\n");
    return;
  }

  carta* tmp= m->inizio_mazzo;
  if(tmp==NULL){
    printf("Mazzo finito\n");
    Termina_partita();
  }
  else if (tmp->next == NULL){//se non c'è una prossima carta
    printf("Hai pescato l'ultima carta del mazzo, se pescherai di nuovo la partita terminerà\n");
    Aggiungi_mano(m, tmp);//si pesca questa carta
    m->inizio_mazzo=NULL;//non ci sono altre carte del mazzo
  }
  else{
    while((tmp->next)->next!=NULL)//finché ci sono carte
      tmp=tmp->next;//si scorre fino alla penultima carta
    Aggiungi_mano(m, tmp->next);//si aggiunge l'ultima carta
    tmp->next=NULL;//si toglie la carta pescata dal mazzo
  }

  pescato=1;
}

static void Aggiungi_mano(mago* m, carta* c){
  short flag=1;
  for (int i = 0; i < 6; i++){
    if(m->mano[i]==NULL){//se la posizione nella mano è libera
      m->mano[i]=c;//si aggiunge alla mano
      printf("Hai pescato ");
      Stampa_carta(c);
      Stampa_mano(m);
      flag=0;
      break;
    }
  }
  if(flag){// se non ci sono posizioni libere la carta è distrutta
    printf("Mano piena, la carta è persa per sempre tra i vicoli di Ravnika\n");
    free(c);
  }
}

static void Gioca(){
  if(giocato){
    printf("Puoi giocare una sola carta per turno!\n");
    return;
  }

  int s;//per la scelta della carta
  short flag=0;//controllo scelta

  Stampa_mano(mago_di_turno);

  do{
    //si controlla che ci sia almeno una creatura con cui attaccare
    short mano_vuota=1;
    int ctr_mano=0;
    for (int i = 0; i < 6; i++) {
      if(mago_di_turno->mano[i]!=NULL){
        mano_vuota=0;
        ctr_mano++;
      }
    }
    if(mano_vuota){
      printf("Non hai carte da giocare in mano!\n");
      return;
    }
    mano_vuota=0;
    printf("\nInserire un numero da 1 a %d per scegliere la carta che vuoi giocare\n", ctr_mano);
    s=Inserisci_numero();

    //controllo
    if(s<1 || s>ctr_mano){
      printf("Il numero deve essere compreso tra 1 e %d\n",ctr_mano);
      flag=1;
    }
    else{
      flag=0;
      carta* carta_giocata=mago_di_turno->mano[s-1];//carta che viene giocata

      //si prende il mago avversario a quello di turno
      mago* mago_avv=&mago1;
      if(mago_di_turno==&mago1)
        mago_avv=&mago2;

      switch (carta_giocata->tipo) {
        case creatura:{
          short giocabile=0;
          for (int i = 0; i < 4; i++){
            if(mago_di_turno->campo[i]==NULL){//la prima posizione libera nella campo
              mago_di_turno->campo[i]=carta_giocata;//si mette la creatura sul campo
              printf("\nHai posizionato %s, creatura con %.1f punti vita nella posizione %d\n\n",carta_giocata->nome, carta_giocata->punti_vita, i+1);
              Stampa_campo(mago_di_turno);
              giocabile=1;
              giocato=1;
              break;
            }
          }
          if(!giocabile){// se non ci sono posizioni libere
            printf("Campo pieno, non puoi giocare una carta creatura\n");
          }
          break;
        }
        case rimuovi_creatura:{
          //si controlla che ci sia almeno una creatura da rimuovere
          short giocabile=0;
          int ctr_campo=0;
          for (int i = 0; i < 4; i++) {
            if(mago_avv->campo[i]!=NULL){
              giocabile=1;
              ctr_campo++;
            }
          }
          if(!giocabile){
            printf("Non ci sono creature avversarie da rimuovere!\n");
            break;
          }

          //si rimuove una creatura a scelta del mago
          short rimossa=0;
          do{
            Stampa_campo(mago_avv);

            int posizione;
            printf("inserire un numero tra 1 e %d per scegliere la creatura avversaria da rimuovere\n",ctr_campo);
            posizione=Inserisci_numero();

            if(posizione<1||posizione>ctr_campo)
              rimossa=0;
            else{
              free(mago_avv->campo[posizione-1]);
              mago_avv->campo[posizione-1]=NULL;
              printf("La creatura avversaria è stata rimossa\n");
              Sistema_campo(mago_avv);
              Stampa_campo(mago_avv);
              rimossa=1;
              giocato=1;
            }
          }while (!rimossa);
          break;
        }
        case infliggi_danno:{
          short flag_i=0;
          //si sceglie il bersaglio da danneggiare
          int bersaglio;
          do{
            printf("A chi vuoi infliggere il danno?\n1 : creatura\n2 : mago avversario\n");
            bersaglio=Inserisci_numero();

            if(bersaglio==1){
              //si controlla che ci sia almeno una creatura da danneggiare
              short giocabile=0;
              int ctr_i=0;
              for (int i = 0; i < 4; i++) {
                if(mago_avv->campo[i]!=NULL){
                  giocabile=1;
                  ctr_i++;
                }
              }
              if(!giocabile){
                printf("Non ci sono creature avversarie da danneggiare!\n");
                break;
              }

              //si sceglie la creatura da danneggiare
              short danneggiata=0;
              do{
                Stampa_campo(mago_avv);

                int p_i;
                printf("inserire un numero tra 1 e %d per scegliere la creatura avversaria da danneggiare\n",ctr_i);
                p_i=Inserisci_numero();

                if(p_i<1||p_i>ctr_i)
                  danneggiata=0;
                else{

                  printf("Hai inflitto %.1f danni a ",carta_giocata->punti_vita);
                  Stampa_carta(mago_avv->campo[p_i-1]);
                  mago_avv->campo[p_i-1]->punti_vita=mago_avv->campo[p_i-1]->punti_vita-carta_giocata->punti_vita;

                  //si controlla che la creatura sia stata distrutta
                  if(mago_avv->campo[p_i-1]->punti_vita<=0){
                    free(mago_avv->campo[p_i-1]);
                    mago_avv->campo[p_i-1]=NULL;
                    printf("La creatura avversaria è stata rimossa\n");
                    Sistema_campo(mago_avv);
                  }
                  danneggiata=1;
                  giocato=1;

                  Stampa_campo(mago_avv);
                }
              }while (!danneggiata);
            }
            else if(bersaglio==2){
              //si picchia il mago
              mago_avv->PV=mago_avv->PV-carta_giocata->punti_vita;
              printf("Hai inflitto %.1f danni al tuo avversario\n",carta_giocata->punti_vita);
              Stampa_PV(mago_avv);
              giocato=1;
              //si controlla che il mago sia morto
              if(mago_avv->PV<=0){
                printf("Hai portato i punti vita del to avversario a zero!\n");
                Termina_partita();
              }
            }
            else{
              printf("Inserire solo 1 o 2\n");
              flag_i=1;
            }
          }while(flag_i);
          break;
        }
        case guarisci_danno:{
          short flag_g=0;
          //si sceglie il bersaglio da guarire
          int bersaglio;
          do{
            printf("Chi vuoi guarire?\n1 : creatura\n2 : me stesso\n");
            bersaglio=Inserisci_numero();

            if(bersaglio==1){
              //si controlla che ci sia almeno una creatura da curare
              short giocabile=0;
              int ctr_campo=0;
              for (int i = 0; i < 4; i++) {
                if(mago_di_turno->campo[i]!=NULL){
                  giocabile=1;
                  ctr_campo++;
                }
              }
              if(!giocabile){
                printf("Non controlli creature da guarire!\n");
                break;
              }

              //si sceglie la creatura da guarire
              short guarito=0;
              do{
                Stampa_campo(mago_di_turno);

                int posizione;
                printf("inserire un numero tra 1 e %d per scegliere la creatura da guarire\n",ctr_campo);
                posizione=Inserisci_numero();

                if(posizione<1||posizione>ctr_campo)
                  guarito=0;
                else{
                  printf("Hai guarito per %.1f punti vita ",carta_giocata->punti_vita);
                  Stampa_carta(mago_di_turno->campo[posizione-1]);
                  mago_di_turno->campo[posizione-1]->punti_vita=mago_di_turno->campo[posizione-1]->punti_vita+carta_giocata->punti_vita;
                  guarito=1;
                  Stampa_campo(mago_di_turno);
                  giocato=1;
                }
              }while (!guarito);
            }
            else if(bersaglio==2){
              //si guarisce il mago
              mago_di_turno->PV=mago_di_turno->PV+carta_giocata->punti_vita;
              printf("Sei guarito %.1f punti vita\n",carta_giocata->punti_vita);
              Stampa_PV(mago_di_turno);
              giocato=1;
            }
            else{
              printf("Inserire solo 1 o 2\n");
              flag_g=1;
            }
          }while(flag_g);
          break;
        default:
          printf("Il sole costituisce il 99 percento della massa del sistema solare\n");
        }
      }
    }
    if(giocato){
      mago_di_turno->mano[s-1]=NULL;//la carta giocata non è più nella mano del mago
      //si sistema la mano
      for (int i = 0; i < 5; i++) {
        if(mago_di_turno->mano[i]==NULL && mago_di_turno->mano[i+1]!=NULL){//se c'è un buco nella mano
          mago_di_turno->mano[i]=mago_di_turno->mano[i+1];//si fa scorrere il buco fino in fondo alla mano
          mago_di_turno->mano[i+1]=NULL;
        }
      }
    }
  }while(flag);
}

static void Attacca(){
  if(attaccato){
    printf("Puoi attaccare una sola volta per turno!\n");
    return;
  }

  int pos_attaccante;//per la scelta della carta
  short scelta_attaccante=0;//controllo scelta

  Stampa_campo(mago_di_turno);

  do{
    //si controlla che ci sia almeno una creatura con cui attaccare
    short attacabile=0;
    int ctr_campo=0;
    for (int i = 0; i < 4; i++) {
      if(mago_di_turno->campo[i]!=NULL){
        attacabile=1;
        ctr_campo++;
      }
    }
    if(!attacabile){
      printf("Non controlli creature con cui attaccare!\n");
      return;
    }

    //si sceglie la creatura cuon cui attacare
    printf("\ninserire un numero da 1 a %d per scegliere la creatura con cui vuoi attaccare\n", ctr_campo);
    pos_attaccante=Inserisci_numero();

    //controllo sulla scelta
    if(pos_attaccante<1 || pos_attaccante>ctr_campo){
      printf("Il numero deve essere compreso tra 1 e %d\n", ctr_campo);
      scelta_attaccante=0;
    }
    else{
      scelta_attaccante=1;
      carta* carta_giocata=mago_di_turno->campo[pos_attaccante-1];//creatura che attacca

      //si prende il mago avversario a quello di turno
      mago* mago_avv=&mago1;
      if(mago_di_turno==&mago1)
        mago_avv=&mago2;

      //si sceglie il bersaglio da attaccare
      int bersaglio=0;
      short scelta_attaccato=0;
      do{
        printf("Chi vuoi attacare?\n1 : creatura\n2 : mago avversario\n");
        bersaglio=Inserisci_numero();

        //si controlla il campo avversario
        short creature=0;
        int ctr_campo_avv=0;
        for (int i = 0; i < 4; i++) {
          if(mago_avv->campo[i]!=NULL){
            creature=1;
            ctr_campo_avv++;
          }
        }

        if(bersaglio==1){
          //si controlla che ci sia almeno una creatura da danneggiare
          if(!creature){
            printf("Non ci sono creature avversarie da attaccare!\n");
            break;
          }

          //si sceglie la creatura da attaccare
          short scelta_creatura_attaccata=0;
          do{
            Stampa_campo(mago_avv);

            int pos_creatura_bersaglio;
            printf("inserire un numero tra 1 e %d per scegliere la creatura avversaria da attaccare\n",ctr_campo_avv);
            pos_creatura_bersaglio=Inserisci_numero();

            if(pos_creatura_bersaglio<1||pos_creatura_bersaglio>ctr_campo_avv)
              scelta_creatura_attaccata=0;
            else{
              printf("Hai inflitto %.1f danni a ",carta_giocata->punti_vita);
              Stampa_carta(mago_avv->campo[pos_creatura_bersaglio-1]);
              mago_avv->campo[pos_creatura_bersaglio-1]->punti_vita=mago_avv->campo[pos_creatura_bersaglio-1]->punti_vita-carta_giocata->punti_vita;

              attaccato=1;
              //si controlla se la creatura sia stata distrutta
              if(mago_avv->campo[pos_creatura_bersaglio-1]->punti_vita<=0){
                free(mago_avv->campo[pos_creatura_bersaglio-1]);
                mago_avv->campo[pos_creatura_bersaglio-1]=NULL;
                printf("La creatura avversaria è stata rimossa\n");
                Sistema_campo(mago_avv);
              }
              Stampa_campo(mago_avv);
              scelta_creatura_attaccata=1;
            }
          }while (!scelta_creatura_attaccata);
          scelta_attaccato=1;
        }
        else if(bersaglio==2){
          //il mago avversario non deve controllare creature
          if(creature){
            printf("Non puoi attaccare il tuo avversario perché ha creture che lo proteggono!\n");
            break;
          }

          //si picchia il mago
          mago_avv->PV=mago_avv->PV-carta_giocata->punti_vita;
          printf("Hai inflitto %.1f danni al tuo avversario\n",carta_giocata->punti_vita);

          Stampa_PV(mago_avv);
          attaccato=1;
          //si controlla che il mago sia morto
          if(mago_avv->PV<=0){
            printf("Hai portato i punti vita del to avversario a zero!\n");
            Termina_partita();
          }
          scelta_attaccato=1;
        }
        else{
          printf("Inserire solo 1 o 2\n");
          scelta_attaccato=0;
        }
      }while(!scelta_attaccato);
    }
  }while(!scelta_attaccante);
}

static void Sistema_campo(mago* m){
  for (int i = 0; i < 3; i++) {
    if(m->campo[i]==NULL && m->campo[i+1]!=NULL){//se c'è un buco nel campo
      m->campo[i]=m->campo[i+1];//si fa scorrere il buco fino in fondo
      m->campo[i+1]=NULL;
    }
  }
}

static void Passa(){
  //cambio mago di turno
  if(mago_di_turno==&mago1)
    mago_di_turno=&mago2;
  else
    mago_di_turno=&mago1;

  Stampa_mago_turno();

  //abbasso le flag
  pescato=0;
  giocato=0;
  attaccato=0;
}

static void Termina_partita(){
  partita_terminata=1;
  printf("\nPartita finita\n");
  if(mago1.PV>mago2.PV){
    printf("Ha vinto %s con %.1f punti vita rimanenti\n",mago1.nome, mago1.PV);
    printf("%s è condannato a diventare una cavia per gli esperimenti della Lega Izzet\n", mago2.nome);
  }
  else if(mago1.PV<mago2.PV){
    printf("Ha vinto %s con %.1f punti vita rimanenti\n",mago2.nome, mago2.PV);
    printf("%s è condannato a diventare una cavia per gli esperimenti della Lega Izzet\n", mago1.nome);
  }
  else{
    printf("Pareggio, entrambi i maghi hanno %.1f punti vita\n", mago1.PV);
  }
}

void Termina_gioco(){
  Dealloca();
  system("clear");
  printf("Il gioco è terminato, attenderò in questa piana ventosa per il prossimo duello\n");
}

static void Dealloca(){
  Dealloca_mazzo(&mago1);
  Dealloca_mazzo(&mago2);
  Dealloca_mano(&mago1);
  Dealloca_mano(&mago2);
  Dealloca_campo(&mago1);
  Dealloca_campo(&mago2);
}

static void Dealloca_mano(mago *m){
  for (int i = 0; i < 6; i++) {
    if(m->mano[i]!=NULL){
      free(m->mano[i]);
      m->mano[i]=NULL;
    }
  }
}

static void Dealloca_campo(mago *m){
  for (int i = 0; i < 4; i++) {
    if(m->campo[i]!=NULL){
      free(m->campo[i]);
      m->campo[i]=NULL;
    }
  }
}

static void Dealloca_mazzo(mago* m) {
  for (int  i = 0; i < n; i++) {//si svuota tutto il mazzo
    if(m->inizio_mazzo != NULL){
      carta* cPrec = NULL;
      carta* tmp = m->inizio_mazzo;
      if(tmp->next == NULL) {// se c'è una sola carta
        free(tmp);
        m->inizio_mazzo= NULL; //visto che il mazzo è vuoto
      }
      else {
        do{
          if((tmp-> next) == m->fine_mazzo) {//siamo arrivati alla penultima carta
            cPrec = tmp;
            break;
          }
          else
            tmp= tmp-> next; //si passa alla prossima
        }while((tmp-> next) != NULL);
        free(cPrec-> next);
        cPrec-> next = NULL; //la carta precedente diventa l'ultima
        m->fine_mazzo = cPrec;
      }
    }
  }
}

static int Inserisci_numero(){
  do{
    char tmp[256];//varaibile stringa di supporto
    fgets(tmp, 256, stdin);

    if(strlen(tmp)>1){//per evitare doppie stampe
      if(strlen(tmp)==2){//si controlla che in input sia stato inserito un solo carattere
        if(tmp[0]>48 && tmp[0]<58){//si controlla che il carattere sia una cifra maggiore di 0
          return((int)tmp[0])-48;
        }
        else
          printf("inserire un numero\n");
      }
      else
        printf("Inserire un numero ad una cifra\n");
    }
  } while(1);
}

void Regole(){
  system("clear");
  printf("Regole base:\n");
  printf("- I giocatori sono due maghi che si sfidano in un duello in cui si combatte usando delle carte\n");
  printf("- Ogni mago inizia sempre con una mano di 5 carte in aggiunta al numero di carte del mazzo\n");
  printf("- Il numero di carte dei mazzi viene scelto nelle impostazioni del gioco\n");
  printf("- Il numero massimo di carte nella mano è 6\n");
  printf("- Se viene pescata una carta quando si ha 6 carte in mano, questa viene distrutta\n");
  printf("- Ogni mago inizia con 20 punti vita\n");
  printf("\n");
  printf("Tipi di carte:\n");
  printf("-\e[0;31m Infliggi danno\e[0m, riduce i punti vita del bersaglio dei punti indicati\n");
  printf("-\e[0;34m Guarisci danno\e[0m, aumenta i punti vita del bersaglio dei punti indicati\n");
  printf("-\e[0;32m Creatura\e[0m, può essere posizionata sul campo e attaccare\n");
  printf("-\e[0;35m Rimuovi creatura\e[0m, elimina una creatura dal campo\n");
  printf("\n");
  printf("Creature:\n");
  printf("- Se i punti vita di una creatura vengono ridotti a zero, la creatura viene rimossa dal campo\n");
  printf("- Non si può attaccare direttamente un mago che controlla creature\n");
  printf("\n");
  printf("Fine della partita:\n");
  printf("- Se un mazzo finisce, la partita termina e vince il mago che a più punti vita\n");
  printf("- Se i punti vita di un mago vengono ridotti a zero, il mago perde\n");
  printf("\n");
}
