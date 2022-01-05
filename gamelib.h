void Imposta_gioco();
void Combatti();
void Regole();
void Termina_gioco();

enum classe_mago {tenebre, vita, luce};

enum tipo_carta {creatura, rimuovi_creatura, infliggi_danno, guarisci_danno};

typedef struct Carta{
  char nome[40];
  enum tipo_carta tipo;
  double punti_vita;
  struct Carta* next;
}carta;

typedef struct Mago{
  char nome[64];
  double PV;
  enum classe_mago classe;
  carta* inizio_mazzo;//carta in fondo
  carta* fine_mazzo; //carta da pescare
  carta* mano[6];
  carta* campo[4];
}mago;
