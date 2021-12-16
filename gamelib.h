void Imposta_gioco();
void Combatti();
void Termina_gioco();

enum classe_mago {tenebre, vita, luce};

enum tipo_carta {creatura, rimuovi_creatura, infliggi_danno, guarisci_danno};

typedef struct Carta{
  enum tipo_carta tipo;
  int punti_vita;
  struct Carta* next;
}carta;

typedef struct Mago{
  char nome[64];
  int PV; //unsigned short
  enum classe_mago classe;
  carta* inizio_mazzo;//carta in fondo - pfirst
  carta* fine_mazzo; //carta da pescare - plast
  carta* mano[6];//array [6]
  carta* campo[4];//array [4]
}mago;
