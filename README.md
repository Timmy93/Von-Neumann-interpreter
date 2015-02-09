# Von-Neumann-interpreter
Is a interpreter of the Von Neumann language written in C.

Progetto Software A.A. 2014/15
Interprete del linguaggio della macchina di von Neumann

Requisiti

Necessari per la compilazione ed esecuzione del programma:
Compilatore C (gcc);
Sistema operativo Linux.

Necessari per utilizzare tutte le funzioni del programma:
File di impostazioni: impo;
Programma scritto secondo le specifiche del linguaggio di von Neumann (VN).
Documentazione tecnica
Lo sviluppo del codice è stato svolto dividendo il progetto in più parti e sviluppandole separatamente.
Inizialmente è stato creato il cuore del programma ovvero la parte di esecuzione del codice di VN allegando solo in seguito altre funzionalità accessorie, gestite tramite un file di impostazioni.
Questo approccio garantisce solidità alla parte centrale del codice perché sviluppato come modulo indipendente; le altre impostazioni non modificano quindi la sua esecuzione.

Fondamentale per il suo corretto funzionamento sono le seguenti funzioni:
int contarighe( FILE *fp )

Si occupa del corretto conteggio del numero di righe presenti nel file fp passato come parametro. La funzione restituisce l'effettivo numero di righe contate, se il file passato non è stato aperto correttamente (fp==NULL) restituisce 0.

int inizio_riga_n( FILE *fp , int num_riga)

Porta il cursore di lettura del file (fp) alla riga desiderata (num_riga).
Restituisce:
0 se non ci sono stati problemi;
-1 se il file non è stato aperto correttamente;
-2 se non esiste la riga richiesta. 

int leggi_riga_n(FILE *fp, char* riga, int num_riga)

Porta il cursore di lettura del file (fp) alla riga desiderata (num_riga) e legge la riga salvandola nell'array di caratteri passato per puntatore (riga).
Restituisce:
0 se non ci sono stati problemi;
-1 se il file non è stato aperto correttamente;
-2 se non esiste la riga richiesta.
int leggi_parola(const char *frase, char *parola)

Salva la prima parola letta dalla frase (frase) nell'array di caratteri parola.
Restituisce il numero di caratteri letti. 
Simulazione della macchina di VN
Per simulare l'accumulatore presente nell'architettura di VN utilizziamo una variabile di tipo intero chiamata acc.
Il nastro di lettura corrisponde agli input da tastiera (stdin).
Il nastro di scrittura corrisponde al terminale (stdout).
L'unita di memoria viene simulata da celle collegate tramite liste.
Ogni volta che si modifica una cella di memoria tramite comando STORE o STORE= si controlla l'esistenza della cella di memoria. Se è assente viene allocata dinamicamente.

Questa è la struttura di una cella di memoria:
struct cella{
    int etic;
    int valore;
    struct cella *prox;
};

etic: è l'etichetta numerica che indica il numero di cella.
valore: è il valore assegnato alla corrispondente cella.
prox: è il puntatore alla cella successiva.
Per debug vengono salvati anche i due nastri in strutture del tipo cella, così da poter vedere in ordine, a fine simulazione, cosa è stato inserito e cosa si stampa a video.

Riconoscimento istruzioni del linguaggio di VN
Per riconoscere le istruzioni ci basta leggere la stringa del comando e la parola iniziale e inviarle alla funzione:
int cmd_chk(const char *cmd, const char *str)

Restituisce:
il numero corrispondente al comando riconosciuto;
-1 se non è stato riconosciuto il comando.

In seguito, tramite un ciclo switch-case, eseguiamo l'istruzione richiesta. Al termine del comando passiamo alla riga successiva (in caso di salto incondizionato o in caso di condizione soddisfatta per il salto condizionato, passiamo alla riga indicata).
Descrizione di ogni fase.
Fase 0 – Boot del programma
Dichiarazione delle variabili.
Inizializzazione di tutte le variabili (in caso di problemi con il file delle impostazioni).
Inizializzazione nastro ingresso, nastro di uscita e memoria della macchina di VN (sono rappresentati tramite liste).
Fase 0.1 – Caricamento impostazioni
Apertura del file di impostazioni (si notifica un eventuale errore in caso di problemi e si salta questa fase mantenendo i valori di default).
Spostamento della testina di lettura all'inizio del file e lettura di ogni riga (tramite un ciclo for e le funzioni: contarighe, leggi_riga_n).
Con la funzione impo_chk si associa ad ogni hotkey un valore numerico e se la riga inizia con una hotkey restituisce il suo valore.
Tramite un ciclo switch-case si aggiorna la variabile associata ad una determinata hotkey del file di impostazioni.
Chiusura del file.
Fase 0.2 – Apertura del file di VN
Controllo se sono stati passati dei parametri da linea di comando.
In caso affermativo si carica il primo e, in caso di parametri multipli, si notifica che ne sono ancora presenti altri.
In caso negativo si chiede all'utente quale programma si vuole eseguire fino a che non si riceve un file valido o l'utente sceglie di terminare l'applicazione.
Per verificare che il file sia valido si prova ad aprirlo in lettura e se non si ricevono errori si prosegue con l'esecuzione del programma.

Fase 1 – Correzione del programma di VN (facoltativo)
Confronto tra il file corretto e il file di origine, se hanno lo stesso nome si cambia il nome del file corretto.
Apertura del file sorgente in lettura e del file di destinazione in scrittura, in caso di errore viene notificato all'utente e si termina l'esecuzione.
Lettura di ogni carattere del file sorgente e trascrizione nel file destinazione, se il carattere è una lettera minuscola si modifica con l'equivalente maiuscola.
Chiusura di entrambi i file.
Nel caso in cui si scelga di non correggere il file utilizzeremo il file sorgente per la simulazione dell'applicazione.
Fase 2 – Controllo correttezza sintattica (facoltativo)
Apertura del file sorgente in lettura. Si posiziona il cursore all'inizio del file, in caso di errore lo si notifica all'utente e si termina l'esecuzione.
Lettura della prima parola di ciascuna riga del file sorgente. 
Tramite la funzione cmd_chk si controlla che la parola in questione sia un comando valido.
Tramite un contatore n inizializzato a 0 si tiene conto del numero di comandi END all'interno del file.
In caso di comando non valido si segnala la riga dell'errore e la parola non riconosciuta. Fine dell'esecuzione.
In caso di mancanza di un END lo si notifica. Fine dell'esecuzione.
Chiusura del file sorgente.
Fase 3 – Pre-allocazione della memoria necessaria
Apertura del file sorgente in lettura, si posiziona il cursore all'inizio del file e in caso di errore lo si notifica all'utente e si termina l'esecuzione.
Lettura di ogni riga del file. Nel caso in cui si trovasse il comando STORE si salva la cella su cui si deve scrivere. 
Per ogni altro comando STORE ci si assicura che la scrittura avvenga su una nuova cella.
Conteggio del numero di celle diverse trovate e allocazione di uno spazio di memoria con l'aggiunta di un elemento alla lista
Chiusura del file sorgente.
FASE 4 – Simulazione del programma di VN
Apertura del file sorgente in lettura, si posiziona il cursore all'inizio del file e in caso di errore lo si notifica all'utente e si termina l'esecuzione.
Se per impostazioni è abilitato il debug, si apre in scrittura il file Debug VN.txt, in caso di errore lo si notifica e si disabilita il debug altrimenti si indica il nome del file di debug.
Lettura della prima parola di ciascuna riga. Tramite la funzione cmd_chk e un ciclo switch-case si riconosce il comando e lo si simula in C.
A fine simulazione si chiude il file sorgente e se il file di debug è stato aperto si stampa il nome delle celle su cui ho lavorato. Chiusura del file.
FASE FINALE – Chiusura del programma
Deallocazione tramite free (utilizzando la funzione libera) della memoria utilizzata dalle liste che sono state create.
Se da terminale, all'avvio del programma, si riceve più di un comando si chiede all'utente se desidera procedere all'esecuzione del programma successivo.
In caso di risposta affermativa si prepara la stringa da passare alla linea di comando per avviare nuovamente il programma utilizzando la funzione system.
La nuova stringa conterrà “./” all'inizio per avviare il programma, poi il nome del programma (sostituendo gli spazi ' ' con dei '\ ') e tutti gli argomenti separati da uno spazio ad eccezione del primo (che sarebbe il programma che abbiamo già eseguito). Questo permetterà di lanciare ricorsivamente il programma fino a che non sono stati simulati tutti gli argomenti passati al software.
Se da impostazioni la variabile conferma è impostata a 1 allora si chiede all'utente l'autorizzazione a terminare il programma.
Descrizione dei comandi di VN:
Riga vuota
Aumenta di 1 la riga.
READ
Legge tramite scanf il valore, lo salva nell'accumulatore e aumenta di 1 la riga. Aggiunge infine il valore alla lista che simula il nastro di lettura.
WRITE
Stampa a schermo tramite printf il valore presente nell'accumulatore. Il valore viene aggiunto alla lista che simula il nastro di scrittura.
END
Termina l'esecuzione del programma e se abilitato dalle impostazioni, stampa a schermo i valori dei due nastri.
LOAD
Legge il valore accanto e lo trasforma in un intero. Tramite la funzione preleva_val copia all'interno dell'accumulatore il valore della cella con l'etichetta letta. Aumenta di 1 la riga.
STORE
Legge il valore accanto, lo trasforma in un intero e tramite la funzione ins_val salva il valore presente nell'accumulatore nella cella con l'etichetta letta. Aumenta di 1 la riga.
ADD
Legge il valore accanto, lo trasforma in un intero e tramite la funzione preleva_val somma il valore della cella con l'etichetta letta al valore dell'accumulatore. Aumenta di 1 la riga.
SUB
Legge il valore accanto, lo trasforma in un intero e tramite la funzione preleva_val sottrae il valore della cella con l'etichetta letta al valore dell'accumulatore. Aumenta di 1 la riga.
MULT
Legge il valore accanto, lo trasforma in un intero e tramite la funzione preleva_val moltiplica il valore della cella con l'etichetta letta al valore dell'accumulatore. Aumenta di 1 la riga.
DIV
Legge il valore accanto, lo trasforma in un intero e tramite la funzione preleva_val viene diviso il valore dell'accumulatore per il valore della cella con l'etichetta letta. Aumenta di 1 la riga.
ADD=
Legge il valore accanto, lo trasforma in un intero e lo somma all'accumulatore. Aumenta di 1 la riga.
SUB=
Legge il valore accanto, lo trasforma in un intero e lo sottrae all'accumulatore. Aumenta di 1 la riga.
MULT=
Legge il valore accanto, lo trasforma in un intero e lo moltiplica all'accumulatore. Aumenta di 1 la riga.
DIV=
Legge il valore accanto, lo trasforma in un intero e divide l'accumulatore per quanto letto. Aumenta di 1 la riga.
LOAD=
Legge il valore accanto, lo trasforma in un intero e lo copia nell'accumulatore. Aumenta di 1 la riga.
BR
Legge il valore accanto, lo trasforma in un intero e imposta la riga a quel valore.
BEQ
Legge il valore accanto, lo trasforma in un intero e imposta la riga a quel valore se l'accumulatore è uguale a 0; altrimenti aumenta di 1 la riga.
BNE
Legge il valore accanto, lo trasforma in un intero e imposta la riga a quel valore se l'accumulatore è diverso da 0; altrimenti aumenta di 1 la riga.
BGE
Legge il valore accanto, lo trasforma in un intero e imposta la riga a quel valore se l'accumulatore è maggiore o uguale a 0; altrimenti aumenta di 1 la riga.

BG
Legge il valore accanto, lo trasforma in un intero e imposta la riga a quel valore se l'accumulatore è maggiore di 0; altrimenti aumenta di 1 la riga.
BLE
Legge il valore accanto, lo trasforma in un intero e imposta la riga a quel valore se l'accumulatore è minore o uguale a 0; altrimenti aumenta di 1 la riga.
BL
Legge il valore accanto, lo trasforma in un intero e imposta la riga a quel valore se l'accumulatore è minore di 0; altrimenti aumenta di 1 la riga.
LOAD@
Legge il comando e il valore accanto ad esso, quest'ultimo viene trasformato in un intero e tramite la funzione ricorsiva store_p ci si riporta al valore della cella con l'etichetta letta (Se ci sono più @ allora la funzione store_p viene richiamata fino a quando non ci si trova nella cella in cui bisogna scrivere). Il comando copia infine il valore presente nella cella puntata nell'accumulatore e aumenta di 1 la riga.
STORE@
Legge il comando e il valore accanto ad esso, quest'ultimo viene trasformato in un intero. La funzione ricorsiva store_p riporta in seguito al valore della cella con l'etichetta letta (Se ci sono più @ allora la funzione store_p viene richiamata fino a quando non ci si trova nella cella in cui bisogna scrivere). Il comando copia infine il valore presente nell'accumulatore nella cella puntata e aumenta di 1 la riga.
Se la cella non esiste nella lista, essa viene creata per opera della funzione ins_t.
Comando non riconosciuto
Termina la simulazione del programma e notifica che non è stato riconosciuto alcun comando.

Documentazione utente

Configurare correttamente il programma tramite il file di impostazioni:
Lista dei parametri e descrizione:
fase1:		Se impostato ad 1 permette di abilitare l'autocorrezione del codice trasformando tutti i comandi di VN in caratteri maiuscoli e creando un file con il codice corretto.
fase2:		Se impostato ad 1 controlla che i comandi inseriti facciano parte del linguaggio di VN. 
debug:		Se impostato ad 1 crea un file chiamato Debug VN.txt in cui si vede riga per riga cosa fa il programma, quante celle di memoria sono state usate e come si modificano durante l'esecuzione.
conferma:	Se impostato ad 1 prima di terminare il programma chiede conferma all'utente.
sostVN:	Chiede il nome del programma che sarà scritto con il codice di VN corretto. Richiede che la fase1 sia attivata.
riga_inizio:	Chiede il numero di riga iniziale del programma di VN.
Un valore errato potrebbe creare errori nei salti del codice (BR, BEQ, …).
mostra_nastri:	Se impostato ad 1 alla terminazione del programma ci mostra i nastri di ingresso e di uscita.
Avvio del programma.
Apriamo un terminale e digitiamo:
$ ./nome_del_programma

In seguito all'avvio ci sarà richiesto quale programma vogliamo simulare:
1)	Se si preme 1 verrà eseguito il programma di default: prog_MVN.
2)	Se si preme 2 verrà terminata l'esecuzione del programma.
3)	Altrimenti si digita il nome del programma da eseguire.
Si può anche avviare uno o più programmi consecutivamente dandoli direttamente tramite riga di comando
$ ./nome_del_programma PROG1 PROG2 ... PROGN

Alla fine dell'esecuzione dei programmi viene richiesto all'utente se si vuole procedere con il successivo o se si vuole terminare l'esecuzione.
