/*
 * ### Consegna: Scrivere un programma C che esegua un programma scritto secondo il linguaggio della macchina di VN.
 * ### Autore: Federico Seri
 * ### Data inizio: 23/10/2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000
#define PROG "prog_MVN"

struct cella{
    int etic;
    int valore;
    struct cella *prox;
};
typedef struct cella Store_el;
typedef Store_el *lista;

int leggi_parola(const char *, char *);
int VNstart(char *);
int contarighe(FILE *);
int inizio_riga_n(FILE *, int);
int leggi_riga_n(FILE *, char *, int);
void trim_space(const char*, char*);
void inizializzazione(lista *);
void ins_t(lista *, int);
void ins_c(lista *, int, int);
void stampa(lista, FILE *);
void nastro(lista);
void libera(lista);
int last_et(lista);
int preleva_val(int, lista);
int ins_val(int, int, lista);
int store_p(char *, int, lista);
int check_var(int, lista);
int cmd_chk(const char *, const char *);
int impo_chk(char *);


/*Ciclo di sostituzione caratteri*/

/*
 * File interessati:
 * - Prog MVN --> Lettura "r"
 * - Prog2 corretto MVN --> Scrittura + lettura "w+"
 * */

int main(int argc, char *argv[]){
	/*Dichiarazione variabili*/
	char car;
	char str[MAX];
	char impo[MAX];
	char sostVN2[MAX];
	char question[MAX];
	char nomeMVN[MAX];
	char sostVN[MAX];
	char cmd[MAX];
	char word[MAX];
	int store[MAX];
	int i, n, num, match, num_store;
	FILE *f1;
	FILE *f2;
	lista start;
	lista ingresso;
	lista uscita;
	int riga, old, acc, temp, fine;
	/*FINE Dichiarazione variabili*/
	
	/*Valori di default in caso di mancanza del file di impostazioni*/
	int inizio=0;
	int fase1=1; /*Fase di verifica che i caratteri siano maiuscoli*/
	int fase2=1; /*Fase di verifica correzione grammaticale*/
	int debug=1; /*Crea un file di debug*/
	int show_nastri=1; /*Ci mostra i nastri alla fine del programma*/
	int conferma=0; /*Conferma di uscita dal programma*/
	strncpy(sostVN, "correzione.vn", MAX-1); /*Nome del prog di VN corretto*/
	/*FINE*/
	
	/*Inizializzazione variabili*/
	strncpy(impo, "impo", MAX-1);
	strncpy(sostVN2, "correzione2.vn", MAX-1); /*Nome del prog di VN corretto - CASO correzione=programma da correggere*/
	strncpy(nomeMVN, "prog_MVN", MAX-1);
	
	inizializzazione(&start);
	inizializzazione(&ingresso);
	inizializzazione(&uscita);
	
	riga=1;	/*Inizierò sempre dalla riga 1, e andrò ad incrementare sia la variabile riga che la variabile inizio man mano*/
	/*FINE Inizializzazione variabili*/
	
/*FASE 0.1: Lettura delle impostazioni*/
    f1=fopen(impo, "r");
    if(f1==NULL){ /*Lettura del file di impostazioni*/
        printf("ERR\tImpossibile aprire %s\n\tVerranno utilizzate le impostazioni predefinite\n\n", impo);
    }
    else{	/*File impostazioni aperto senza problemi, procedo alla lettura delle informazioni*/
		fseek(f1, 0L, SEEK_SET);
		for(i=1; i<contarighe(f1)+1; i++){ /*Cicla fino alla fine del file per verificare la correttezza delle varie istruzioni*/
			leggi_riga_n(f1, str, i);
			n=leggi_parola(str, cmd);
				
			switch(impo_chk(cmd)){
				case 1: /*FASE1*/
					leggi_parola(str+n+1, cmd);
					fase1=atoi(cmd);
					break;
				case 2: /*FASE2*/
					leggi_parola(str+n+1, cmd);
					fase2=atoi(cmd);
					break;
				case 3: /*DEBUG*/
					leggi_parola(str+n+1, cmd);
					debug=atoi(cmd);
					break;
				case 4: /*CONFERMA*/
					leggi_parola(str+n+1, cmd);
					conferma=atoi(cmd);
					break;
				case 5: /*sostVN*/
					leggi_parola(str+n+1, cmd);
					strncpy(sostVN, cmd, MAX-1);
					break;
				case 6: /*riga_inizio*/
					leggi_parola(str+n+1, cmd);
					inizio=atoi(cmd);
					break;
				case 7: /*show_nastri*/
					leggi_parola(str+n+1, cmd);
					show_nastri=atoi(cmd);
					break;
				default:
					break;
			}
		} /*Chiudo il FOR*/
		fclose(f1);
		printf("OK\tImpostazioni caricate correttamente\n");
	}

/*FASE 0.2: Lettura parametri passati*/
		n=0;
		if(argc>1){	/*Ho ricevuto il programma da aprire*/
			strncpy(nomeMVN, argv[1], MAX-1);
		}
		else {	/*NON ho ricevuto il programma da aprire -> Cerco il programma di default*/
			do{
				printf("\nScrivi il nome del file che vuoi aprire\n");
				printf("Scrivi 1 per aprire il programma di default\n");
				printf("Scrivi 2 per uscire dal programma\n");
				printf("La mia scelta è: \t");
				fgets(question, MAX, stdin);
				for(n=0; n<strlen(question); n++){	/*Cancella tutti gli a capo*/
					if( question[n] == '\n' )
						question[n] = '\0';
				}
				i=atoi(question);
				/*È stato scelto il file di default*/
				if(i==1){
					break;	/*Apriremo il programma di default*/
				}
				/*Si è deciso di uscire*/
				else if(i==2){
					printf("Hai scelto di uscire\n");
					return 0;
				}
				else{
					f1=fopen(question, "r");
					if(f1==NULL){
						printf("ERR\tImpossibile aprire il file:\t%s\n", question);
					}
					else{	/*OK va tutto bene, rilascio il file ed esco*/
						fclose(f1);
						strncpy(nomeMVN, question, MAX-1);	/*Facciamo in modo che apra il programma scelto*/
						break;	/*Esco perchè non ho avuto problemi nell'apertura del file*/
					}
				}
			}while(1);
		}
		printf("OK\tApertura: %s\n", nomeMVN);
		if(argc>2){
			printf("INFO:\tMi hai passato %d programmi\n\tEseguirò il primo\n\n", argc-1);
		}
/*FINE FASE 0*/

if(fase1){
/*FASE 1: Pulizia file MVN e riscrittura del file corretto*/
	/*Controllo che i file da correggere e il nuovo file non corrispondano*/
	if(!strcmp(nomeMVN, sostVN))	/*Se i due file sono uguali evitiamo di riscrivere sopra al file che stiamo andando a leggere*/
		strncpy(sostVN, sostVN2, MAX-1);

	/*Apertura file*/
	f1=fopen(nomeMVN, "r");
	if(f1==NULL){
		printf("Impossibile aprire il file: %s\nTermino l'esecuzione\n", nomeMVN);
		return 0;
	}
	f2=fopen(sostVN, "w");
	if(f2==NULL){
		printf("Impossibile scrivere il file: %s\nTermino l'esecuzione\n", sostVN);
		return 0;
	}
	/*Ciclo di sostituzione caratteri*/
	while(!feof(f1)){
		fscanf(f1, "%c", &car);
		if(car<='z' && car>='a')
			car-='a'-'A';
		fprintf(f2, "%c", car);
	}
	/*Chiusura file*/
	fclose(f1);
	fclose(f2);
/*FINE FASE 1*/
}
else{	/*Caso in cui salto la correzione del file*/
	strncpy(sostVN, nomeMVN, MAX-1);	/*Carico il file originale come sorgente da cui leggere*/
}

if(fase2){
/*FASE 2: Controllo correttezza file sintattica*/
	/*Analizzo ogni riga e vedo se esiste ogni stringa*/
	f1=fopen(sostVN, "r");
	if(f1==NULL){
		printf("Impossibile aprire il file: %s\nTermino l'esecuzione\n", sostVN);
		return 0;
	}
	fseek(f1, 0L, SEEK_SET);
	n=0; /*Numero di END trovati*/
	for(i=0; i<contarighe(f1); i++){ /*Cicla fino alla fine del file per verificare la correttezza delle varie istruzioni*/
		leggi_riga_n(f1, str, i);
		leggi_parola(str, cmd);
		if(strcmp(cmd, "END")==0)
			n++;
		if(cmd_chk(cmd, str)==-1){
			printf("Comando alla riga %d: '%s' non riconosciuto\nImpossibile interpretare il codice\n", i, cmd);
			return 0;
		}
	}

	if(n<1){
		printf("###ATTENZIONE###\nNon e' presente alcun comando END per la terminazione del programma\nImpossibile svolgere simulazione\n");
		return 0;
	}
	fclose(f1);
/*FINE FASE 2*/
}

/*FASE 3: Conteggio STORE e dichiarazione dinamica delle variabili*/
	/*Apertura file*/
	f1=fopen(sostVN, "r");
	if(f1==NULL){
		printf("Impossibile aprire il file: %s\nTermino l'esecuzione\n", sostVN);
		return 0;
	}
	fseek(f1, 0L, SEEK_SET); /*Riporto all'inizio sostVN*/
	num_store=0;

	/*###Contiamo gli store###*/
	for(i=1; leggi_riga_n(f1, str, i)==0; i++){ /*Resta nel ciclo fino a che non legge tutte le righe*/
		leggi_parola(str, word);
		if(!strcmp("STORE", word)){ /*Entra nell'if quando legge uno STORE*/
			leggi_parola(str+6, word); /*La variabile letta viene salvata in word*/
			num=atoi(word); /*La variabile viene salvata come numero*/

			if(num_store==0){ /*E' il primo store incontrato*/
				store[num_store]=num; /*Salva la prima variabile letta nell'array*/
				num_store++;
			}
			else{ /*Confrontiamo la variabile con tutte quelle salvate nell'array e se non compare la aggiungiamo all'array*/
				match=0; /*Non abbiamo controllat niente => La variabile è ancora nulla*/
				for(n=0; n<num_store; n++){
					if(num==store[n]){
						match=1; /*Esiste già questa variabile => Lo segnamo e non verrà aggiunta all'array*/
					}
				}
				if(match==0){
					store[num_store]=num; /*Aggiungiamo la var all'array e aumentiamo di uno il contatore num_store*/
					num_store++;
				}
			}
		}
	}

	/*###Allochiamo dinamicamente la memoria richiesta dal numero di STORE e assegnamo le etichette###*/
	for(i=0; i<num_store; i++){
		ins_t(&start, store[i]);
	}

	/*Chiusura file*/
	fclose(f1);
/*FINE FASE 3*/

/*FASE 4: Simulazione del programma di VN*/
	printf("\n###Inizio simulazione###\n");
	f1=fopen(sostVN, "r");
	if(f1==NULL){
		printf("Impossibile aprire il file: %s\nTermino l'esecuzione\n", sostVN);
		return 0;
	}
	/*Apertura File per scrittura info*/
	if(debug){
		f2=fopen("Debug VN.txt","w");
		if(f2==NULL){
			printf("##Errore nella scrittura del file di debug##\n");
			debug=0; /*Impossibile scrivere il debug*/
		}
		else
			fprintf(f2, "Debug del programma: %s", nomeMVN);	/*Inizio a scrivere il file di debug*/
	}
	/*###NEW*/
	fine=0;
	while(1){
		if(fine==1)
			break; /*Esco dal ciclo se ho letto un END*/
		else if(fine==-1)
			return 0; /*Errore nella lettura*/
		
		if(leggi_riga_n(f1, str, riga)==-2){	/*Iniziamo a leggere le righe*/
			/*Se siamo qui allora stiamo leggendo una riga inesistente*/
			printf("\nImpossibile trovare la riga %d\nTermino la simulazione del programma\n", riga);
			break;
		}
		i=leggi_parola(str, cmd);
		if(debug)	fprintf(f2, "\n#%d:\t%s\t", riga, cmd);
		printf("Riga: %d\n", riga-1+inizio);

		switch(cmd_chk(cmd, str)){
			case 1: /*Case: READ*/
				/*printf("Lettura da nastro:\t");*/
				scanf("%d", &acc);
				riga++;
				/*Ora c'è la parte di allocazione per la lista di uscita*/
				if(uscita==NULL)
					old=0;
				else
					old=last_et(ingresso);	/*Ci preleva l'ultima etichetta*/
				ins_c(&ingresso, old+1, acc);	/*Ci inserisce il valore in coda*/
				break;
			case 2:/*Case: RIGA VUOTA*/
				riga++;
				break;
			case 3: /*Case: WRITE*/
				printf("%d\n", acc);
				riga++;
				/*Ora c'è la parte di allocazione per la lista di uscita*/
				if(uscita==NULL)
					old=0;
				else
					old=last_et(uscita);	/*Ci preleva l'ultima etichetta*/
				ins_c(&uscita, old+1, acc);	/*Ci inserisce il valore in coda*/
				break;
			case 4: /*Case: END*/
				fine=1;
				printf("\n###Fine simulazione###\n");
				if(show_nastri){
					/*Ora c'è la parte in cui ci occupiamo di stampare i nastri*/
					printf("Nastro di ingresso\n");
					nastro(ingresso);
					printf("Nastro di uscita\n");
					nastro(uscita);
					printf("\n");
				}
				break;
			case 5: /*Case: LOAD*/
				leggi_parola(str+1+i, word); /*Lettura parametro*/
				temp=atoi(word); /*Conversione in numero del parametro*/
				old=acc;
				acc=preleva_val(temp, start);
				riga++;
				if(debug)	fprintf(f2, "Accumulatore: %d ==> %d", old, acc);
				break;
			case 6: /*Case: STORE*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				old=preleva_val(temp, start); /*Vecchio valore della cella*/
				ins_val(temp, acc, start);
				riga++;
				if(debug)	fprintf(f2, "Cella %d: %d ==> %d", temp, old, acc);
				break;
			case 7: /*Case: ADD*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				old=acc;
				acc+=preleva_val(temp, start); /*###OPERAZIONE###*/
				riga++;
				if(debug)	fprintf(f2, "Accumulatore: %d ==> %d", old, acc);
				break;
			case 8: /*Case: SUB*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				old=acc;
				acc-=preleva_val(temp, start);/*###OPERAZIONE###*/
				riga++;
				if(debug)	fprintf(f2, "Accumulatore: %d ==> %d", old, acc);
				break;
			case 9: /*Case: MULT*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				old=acc;
				acc*=preleva_val(temp, start);/*###OPERAZIONE###*/
				riga++;
				if(debug)	fprintf(f2, "Accumulatore: %d ==> %d", old, acc);
				break;
			case 10: /*Case: DIV*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				old=acc;
				acc=acc/(preleva_val(temp, start));/*###OPERAZIONE###*/
				riga++;
				if(debug)	fprintf(f2, "Accumulatore: %d ==> %d", old, acc);
				break;
			case 11: /*Case: ADD=*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				old=acc;
				acc+=temp; /*###OPERAZIONE###*/
				riga++;
				if(debug)	fprintf(f2, "Accumulatore: %d ==> %d", old, acc);
				break;
			case 12: /*Case: SUB=*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				old=acc;
				acc-=temp;/*###OPERAZIONE###*/
				riga++;
				if(debug)	fprintf(f2, "Accumulatore: %d ==> %d", old, acc);
				break;
			case 13: /*Case: MULT=*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				old=acc;
				acc*=temp;/*###OPERAZIONE###*/
				riga++;
				if(debug)	fprintf(f2, "Accumulatore: %d ==> %d", old, acc);
				break;
			case 14: /*Case: DIV=*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				old=acc;
				acc/=temp;/*###OPERAZIONE###*/
				riga++;
				if(debug)	fprintf(f2, "Accumulatore: %d ==> %d", old, acc);
				break;
			case 15: /*Case: LOAD=*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				if(debug)	fprintf(f2, "Accumulatore: %d ==> %d", acc, temp);
				acc=temp;/*###OPERAZIONE###*/
				riga++;
				break;
			case 16: /*Case: BR*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				riga=temp+1-inizio;/*###OPERAZIONE###*/
				break;
			case 17: /*Case: BEQ*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				if(acc==0)
					riga=temp+1-inizio;/*###OPERAZIONE###*/
				else riga++;
				break;
			case 18: /*Case: BNE*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				if(acc!=0)
					riga=temp+1-inizio;/*###OPERAZIONE###*/
				else riga++;
				break;
			case 19: /*Case: BGE*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				if(acc>=0)
					riga=temp+1-inizio;/*###OPERAZIONE###*/
				else riga++;
				break;
			case 20: /*Case: BG*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				if(acc>0)
					riga=temp+1-inizio;/*###OPERAZIONE###*/
				else riga++;
				break;
			case 21: /*Case: BLE*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				if(acc<=0)
					riga=temp+1-inizio;/*###OPERAZIONE###*/
				else riga++;
				break;
			case 22: /*Case: BL*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				if(acc<0)
					riga=temp+1-inizio;/*###OPERAZIONE###*/
				else riga++;
				break;
			case 23: /*Case: LOAD@*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				temp=store_p(cmd, temp, start);
				acc=preleva_val(temp, start);
				riga++;
				break;
			case 24: /*Case: STORE@*/
				leggi_parola(str+1+i, word);
				temp=atoi(word);
				temp=store_p(cmd, temp, start);
				if(check_var(temp, start)!=0)
					ins_t(&start, temp);
				ins_val(temp, acc, start);
				riga++;
				break;
			default:
				printf("Comando:'%s' non presente\nIstruzione:'%s' non riconosciuta\nTermino la simulazione del programma alla riga: %d\n", cmd, str, riga);
				fine=-1;
				break;
		}
	}
	/*FINE FASE 4*/
	
	/*FASE FINALE*/
	fclose(f1);
	if(debug){
		stampa(start, f2);
		fclose(f2);
	}
	/*Liberiamo la memoria allocata - Non ci serve più*/
	libera(start);
	libera(ingresso);
	libera(uscita);
	
	if(argc>2){
		i=1;
		printf("\nQuando sei pronto per eseguire il prossimo programma premi 0\nL'esecuzione cancellerà tutti i file di debug salvati\nPremi un altro tasto per Annullare\nScelta\t");
		scanf("%d", &i);
		if(i==0){ /*Andiamo avanti se l'utente ha scelto di proseguire*/
			/*Faccio in modo che gli spazi ' ' diventino '\ ' per poter dare il comando al terminale*/
			trim_space(argv[0], impo);			
			
			/*Se ci sono altri argomenti li aggiungiamo in fondo dopo averli ripuliti*/
			for(n=2; n<argc; n++){
				trim_space(argv[n], question);	/*Ripuliamo gli spazi*/
				strncat(impo, " ", 1);				/*Colleghiamo gli argomenti mettendo uno spazio*/
				strncat(impo, question, strlen(question));			/*Inseriamo l'argomento*/
			}
			
			system("clear");
			printf("Esecuzione del comando:\t%s\n\n", impo);
			system(impo);
		}
	}

	if(conferma){
		printf("Premi un tasto per terminare l'esecuzione");
		scanf("%d", &i);
	}
	return 0;
}

/*FUNZIONI UTENTE*/

/*FUNZIONI Stringhe*/
int leggi_parola(const char *frase, char *parola){
	int i;
	int n=0;
	for(i=0; (frase[i]!=' ' && frase[i]!='\n' && frase[i]!='\0'); i++){
		parola[n]=frase[i];
		n++;
	}
	parola[n]='\0';
	return i; /*Ritorna la posizione di lettura*/
}

int VNstart(char *stringa){ /*Riceve in ingresso una stringa di caratteri e restituisce la posizione del primo carattere dopo un etichetta del tipo #etic#*/
	int n=0;
	if((stringa[n])!='#')
		return n;
	else{
		n++;
		while(stringa[n]!='#')
			n++;
	}
	return n+1;
}

int contarighe(FILE *fp){
	char car;
	int j;
	if(fp==NULL)	/*Se il file non è aperto ci torna 0 righe*/
		return 0;
	j=0;
	fseek(fp, 0L, SEEK_SET);	/*Torniamo all'inizio*/
	while(!feof(fp)){
		car=fgetc(fp);
		if(car=='\n')
			j++;
	}
	return j;
}

int inizio_riga_n(FILE *fp, int num_riga){ /*Sposta il cursore al primo carattere della riga desiderata ritorna 0 salvo errori*/
	char car;
	int n=1; /*Numero riga raggiunta*/
	
	/*Verifichiamo che il file sia aperto*/
	if(fp==NULL)
		return -1;
	/*Verifichiamo che quella riga esista*/
	if(contarighe(fp)<num_riga)
		return -2; /*Ci sono meno righe del previsto*/

	/*Ci portiamo all'inizio del file per iniziare a contare gli a capo*/
	fseek(fp, 0L, SEEK_SET); 
	while(n<num_riga){
		car=fgetc(fp);;
		if(car=='\n')
			n++;
	}
	ftell(fp);
	return 0; /*Ci torna il primo carattere dopo l'a capo*/
}

int leggi_riga_n(FILE *fp, char* riga, int num_riga){ /*Legge la riga desiderata e ritorna 0 salvo errori*/
	/*Verifichiamo che il file sia aperto*/
	if(fp==NULL)
		return -1;
	/*Verifichiamo che quella riga esista*/
	if(contarighe(fp)<num_riga)
		return -2; /*Ci sono meno righe del previsto*/
	inizio_riga_n(fp, num_riga);
	fgets(riga, MAX, fp);
	return 0;
}

void trim_space(const char* vecchio, char* nuovo){ /*Prendiamo la vecchia riga e copiamo nella nuova riga tutti i caratteri sostituendo gli spazi*/
	int n;
	int num=0;	/*Inizializzo il contatore di spazi*/
	for(n=0; n<strlen(vecchio); n++){
		if(vecchio[n]==' '){
			nuovo[n+num]='\\';	/*Metto uno slash*/
			nuovo[n+1+num]=vecchio[n];	/*Copia lo spazio*/
			num++;
		}
		else{
			nuovo[n+num]=vecchio[n];
		}
	}
	nuovo[n+num]='\0';	/*Terminiamo la riga dopo aver scritto la parola*/
	return;
}

/*FINE Stringhe*/

/*FUNZIONI Liste*/
void inizializzazione(lista *puntatorealprimo){
	*puntatorealprimo=NULL; /*Il puntato da start viene messo a NULL*/
	}

void ins_t(lista *puntatoreallatesta, int etich){		/*Inserisci un elemento come 1°*/
	lista p;
	/*Creazione elemento*/
	p=(lista) malloc(sizeof(Store_el)); /*Allochiamo la memoria della dimensione di una nuova cella di memoria*/
	p->etic=etich; /*Assegnamo l'etichetta*/
	p->prox=*puntatoreallatesta; /*L'elemento punterà a quanto puntato prima da puntatoreallatesta*/
	*puntatoreallatesta = p; /*L'elemento sarà puntato da start*/
	}

void ins_c(lista *puntatoreallatesta, int etich, int val){	/*Inserisce un elemento come ultimo della lista*/
	if(*puntatoreallatesta==NULL){	/*È il caso in cui siamo all'ultimo elemento*/
		lista p;
		/*Creazione elemento*/
		p=(lista) malloc(sizeof(Store_el)); /*Allochiamo la memoria della dimensione di una nuova cella di memoria*/
		*puntatoreallatesta=p;	/*Abbiamo collegato il precedente con questo*/
		p->etic=etich;	/*Impostiamo l'etichetta*/
		p->valore=val;	/*Impostiamo il valore*/
		p->prox=NULL;	/*Impostiamo il prossimo*/
	}
	else{
		ins_c(&((*puntatoreallatesta)->prox), etich, val);	/*Rifacciamo il ciclo andando al prossimo elemento*/
	}
	
}

int preleva_val(int etic, lista punt){/*Riceve in ingresso un etichetta e ci restitusce il valore*/
	int i;
	for(i=0; punt->etic!=etic; i++){
		punt=punt->prox;
		if(punt==NULL && punt->etic!=etic)
			printf("Attenzione - Errore\n");
	}
	return punt->valore;
}

int ins_val(int etic, int val, lista punt){ /*Riceve in ingresso un etichetta e un valore e aggiorna il valore*/
	int i;
	for(i=0; punt->etic!=etic; i++){
		punt=punt->prox;
		if(punt==NULL && punt->etic==etic){
			return -1;
		}
	}
	(punt->valore)=val;
	return 0;
}

int store_p(char *str, int val, lista start){ /*Svolgo tutti i puntaggi e ritorno il valore finale*/
	if(str[strlen(str)-1]=='@'){ /*C'è ancora un assegnamento*/
		val=preleva_val(val, start);
		str[strlen(str)-1]='\0';
		val=store_p(str, val, start);
		return val;
	}
	else{ /*Ora è un semplice STORE VAL*/
		return val;
	}
}

int check_var(int etic, lista start){	/*Controlla se ho già allocato una variabile*/
	int i;
	int check=-1;
	for(i=0; start != NULL ; i++){
		if(start->etic==etic)
			check=0;
		start=start->prox;
	}
	return check;
}

void stampa(lista puntatoreallatesta, FILE *fp){		/*Stampo tutta la lista su file*/
	int i;
	fprintf(fp, "\nCelle di memoria utilizzate durante l'esecuzione:\n");
	for(i=0; puntatoreallatesta != NULL; i++){
		fprintf(fp, "Cella %d\n", puntatoreallatesta->etic);
		puntatoreallatesta=puntatoreallatesta->prox;
	}
}

int last_et(lista puntatoreallatesta){
	if(puntatoreallatesta==NULL)	/*C'è un errore -> NON DEVE MAI ESSERE NULL*/
		return -1;
	else if(puntatoreallatesta->prox==NULL){	/*È l'ultimo valore*/
		return puntatoreallatesta->etic;
	}
	else
		return last_et(puntatoreallatesta->prox);
}
		
void nastro(lista p){
		if(p==NULL){
			printf("\n");
			return;
		}
		else if(p!=NULL){
			printf("%d", p->valore);
			if(p->prox!=NULL)
				printf(" - ");
			nastro(p->prox);
			return;
		}
}

void libera(lista p){
	if(p==NULL){	/*Siamo arrivati in fondo iniziamo a tornare indietro*/
		return;
	}
	else{
		libera(p->prox);
		free(p);	/*Quando siamo di ritorno liberiamo la cella*/
		return;
	}
	
}
/*FINE liste*/

/*FUNZIONI parsing*/
int cmd_chk(const char *cmd, const char *str){
	int var;
	if(strcmp(cmd, "READ")==0) var=1;
	else if(str[0]=='\n') var=2;
	else if(strcmp(cmd, "WRITE")==0) var=3;
	else if(strcmp(cmd, "END")==0) var=4;
	else if(strcmp(cmd, "LOAD")==0) var=5;
	else if(strcmp(cmd, "STORE")==0) var=6;
	else if(strcmp(cmd, "ADD")==0) var=7;
	else if(strcmp(cmd, "SUB")==0) var=8;
	else if(strcmp(cmd, "MULT")==0) var=9;
	else if(strcmp(cmd, "DIV")==0) var=10;
	else if(strcmp(cmd, "ADD=")==0) var=11;
	else if(strcmp(cmd, "SUB=")==0) var=12;
	else if(strcmp(cmd, "MULT=")==0) var=13;
	else if(strcmp(cmd, "DIV=")==0) var=14;
	else if(strcmp(cmd, "LOAD=")==0) var=15;
	else if(strcmp(cmd, "BR")==0) var=16;
	else if(strcmp(cmd, "BEQ")==0) var=17;
	else if(strcmp(cmd, "BNE")==0) var=18;
	else if(strcmp(cmd, "BGE")==0) var=19;
	else if(strcmp(cmd, "BG")==0) var=20;
	else if(strcmp(cmd, "BLE")==0) var=21;
	else if(strcmp(cmd, "BL")==0) var=22;
	else if(strncmp(cmd, "LOAD@", strlen("LOAD@"))==0) var=23;
	else if(strncmp(cmd, "STORE@", strlen("STORE@"))==0) var=24;

	else var=-1;
	return var;
}

int impo_chk(char *cmd){
    int var;
	if(strcmp(cmd, "fase1")==0) var=1;
    else if(strcmp(cmd, "fase2")==0) var=2;
    else if(strcmp(cmd, "debug")==0) var=3;
    else if(strcmp(cmd, "conferma")==0) var=4;
    else if(strcmp(cmd, "sostVN")==0) var=5;
    else if(strcmp(cmd, "riga_inizio")==0) var=6;
    else if(strcmp(cmd, "mostra_nastri")==0) var=7;
    else var=-1;
    return var;
}

/*FINE parsing*/
