# Tragic: Un gioco di carte
progetto finale Programmazione Procedurale con Laboratorio a.a. 2021/2022.

In questo README sono elencati eventuali estensioni al progetto assegnato, e scelte progettuali.


Nome: Iuri

Cognome: Fulgidi

Matricola: 340803


Scelte e implementazioni aggiuntive al progetto;
- Per il mazzo di carte è stato imposto un range da 0 a 80 carte per evitare
mazzi negativi e partite troppo lunghe.
- Per simulare il fatto che le carte più potenti si trovano in fondo al mazzo, questo viene diviso in tre parti uguali:
  -una iniziale in cui i punti vita vanno da 7 a 10
  -una intermedia in cui i punti vita vanno da 4 a 7
  -una finale in cui i punti vita vanno da 1 a 4
I punti vita delle carte in mano seguono i criteri della sezione finale, ovvero le carte che saranno pescate per prime.
Questi sono comunque i valori di base che vengono poi modificati in base alla classe del mago.
- É stata aggiunta una funzione Regole al menu principale nel main che permette di vedere le regole del gioco
