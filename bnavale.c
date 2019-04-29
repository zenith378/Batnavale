//
//  bnavale.c
//
//
//  Created by Giulio Cordova and Jacopo Acquistapace on 10/04/19.
//
//  Si dichiara che il contenuto di questo file e' in ogni sua parte opera originale degli autori eccetto dove specificato.

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bnavale.h"


void empty_stdin (void) /* Funzione utilizzata per svuotare la stdin quando si usa la scanf per leggere un carattere inserito dall'utente in momenti precisi del gioco */
{
    int c = getchar();
    
    while (c != '\n' && c != EOF) //finché non si digita invio o non si arriva alla fine del file la funzione prende un nuovo carattere in stdin.
        c = getchar();
}

/*Schermata di benvenuto. Il disegno è di sagnew https://github.com/sagnew/Battleship
 La funzione permette di leggere le regole contenute nel file "regole.txt"
 */

void superwelcomescreen(){
    FILE * file; // file regole del gioco
    int micro=200000; // tempo di attesa in microsecondi tra la stampa di una riga dell'immagine e la successiva
    int c;
    printf("                                  _/|                           \n");
    usleep(micro);
    printf("                                 _|:|                           \n");
    usleep(micro);
    printf("                               _|:::|                           \n");
    usleep(micro);
    printf("                         _ /|  |::::|                           \n");
    usleep(micro);
    printf("                         |::|  |::::|                           \n");
    usleep(micro);
    printf("                         |::|  |:::::)__                        \n");
    usleep(micro);
    printf("                       /:|:::: |::::::::|                       \n");
    usleep(micro);
    printf("                      :::|::::_|::::::::|                       \n");
    usleep(micro);
    printf("                ____(:::::::::::::::::::|___ _______         __ \n");
    usleep(micro);
    printf("               (::::::::::::::::::::::::|:::|::::::|  ______|::)\n");
    usleep(micro);
    printf("     ___________\\:::::::::::::::::::::::::::|:::|___|:::::::::|\n");
    usleep(micro);
    printf("     \\::::::::::::::::::::::::::::::::::::::::::::::::::::::::|\n");
    usleep(micro);
    printf("      |:::::::::::::::::::::::::::::::::::::::::::::::::::::::| \n");
    usleep(micro);
    printf("__________          __    __  .__                  .__    .__          \n");
    usleep(micro);
    printf("\\______   \\_____  _/  |__/  |_|  |    ____   ______|  |__ |__|______ \n");
    usleep(micro);
    printf(" |    |  _/\\__  \\ \\   __\\   __\\  |  _/ __ \\ /  ___/|  |  \\|  |\\____ \\  \n");
    usleep(micro);
    printf(" |    |   \\ / __ \\_|  |  |  | |  |__\\  ___/ \\___ \\ |      \\  ||  |_> >    \n");
    usleep(micro);
    printf(" |______  /(______/|__|  |__| |____/ \\_____>______>|___|  /__||   __/          \n");
    usleep(micro);
    printf("        \\/                                              \\/    |__|            \n");
    sleep(1);
    printf("###############################################################\n");
    printf("#################                                         #####\n");
    printf("#################       Benvenuto a Battaglia Navale 1.0  #####\n");
    printf("#################                                         #####\n");
    printf("#################               Premi invio per iniziare  #####\n");
    printf("###############################################################\n");
    
    empty_stdin(); // pulisce la stdin dopo che l'utente ha premuto INVIO
    printf("\nPremi 0 e invio per visualizzare le regole. Premi un qualsiasi altro tasto e invio per giocare\n");
    
    c=getchar(); // prende in stdin il carattere inserito dall'utente per avviare il gioco o leggere le regole
    if(c=='0') { //Entro solo se il carattere digitato è 0.
        file = fopen("regole.txt", "r");
        if (file!=NULL) { //mi assicuro di non leggere un NULL, ad esempio in caso di mancanza del file nella cartella
            while ((c = getc(file)) != EOF) putchar(c); //leggo fino alla fine del file
            fclose(file);
        }
        else printf("\nSei sicuro di aver salvato il file regole.txt nella stessa cartella del file bnavale.c?\n"); //messaggio di errore in caso fopen restituisca NULL
    }
    empty_stdin(); // pulisce la stdin
}
/* funzione BUFFER: ha lo scopo di controllare il corretto funzionamento della funzione scanf in tutti i casi in cui viene letto ed assegnato un valore inserito dall'utente in stdin durante il gioco */

int buffer (int rtn){/*Gestione buffer: rtn è l'intero restituito dalla scanf: se scanf ritorna 0, significa che non sono stati letti e asseganti valori, altrimenti restituisce un numero intero corrispondente al numero di valori letti ed assegnati*/
    if (rtn == 0) {    //caso 0: non sono stati assegnati valori: stampo messaggio di errore e ritorno -1.
        printf ("\nDevi inserire un numero.\n");
        empty_stdin(); //pulisce la stdin
        return -1;
    }
    else {  /* input corretto: ritorno 0. */
        empty_stdin(); // pulisce la stdin
        return 0;
    }
    
}

/* Funzione che inizializza l'area di gioco di una generica struct sea. Si veda bnavale.h per dettagli. */

void init (area_t * sea){
    area_t * mare=sea; //gestisco variabili locali.
    int i,j,n=mare->N,m=mare->M; //n numero di righe, m colonne
    
    char ** AREA=mare->a; //definisco la matrice come un'array di puntatori, per cui il primo elemento è un puntatore a puntatore.
    for (i=0; i<n; i++){
        for (j=0;j<m; j++) /* i due cicli for annidati assegnano il valore EMPTY a tutti gli elementi della matrice sea */
            AREA[i][j]=EMPTY;
    }
    mare->n_navi=0;//assegno 0 al numero di navi contenute nella matrice.
}

/*Funzione che crea e inizializza una struct sea. Si veda navale.h per dettagli.  */

area_t * crea_area (unsigned n, unsigned m){
    
    int i;
    char ** AREA;
    
    area_t * mare=NULL; //inizializzo il puntatore a NULL.
    mare = (area_t*)malloc(sizeof(area_t)); //alloco il puntatore alla struct.
    AREA=malloc((n)*sizeof(char * )); //alloco la prima colonna della matrice AREA come un array di puntatori a char di lunghezza n.
    if (mare==NULL||AREA==NULL) return NULL;//gestione del NULL prima di leggere i campi della struct, evitando errore di segmentazione.
    //inizializzazione dei campi della struct. Si veda bnavale.h per dettagli.
    mare->N=n;
    mare->M=m;
    mare->a=AREA;
    mare->hit=0;//#pezzi di nave colpiti
    mare->misses=0;//#pezzi di navi mancate
    mare->shots=0;//#colpi totali
    
    for (i=0; i<n; i++) {
        AREA[i]=malloc((m)*sizeof(char));
        if(AREA[i]==NULL) return NULL;
    } //alloco tutti gli array puntati da AREA come array di lunghezza m contenti caratteri.
    
    init(mare); //inizializzo la matrice ad EMPTY
    
    return mare; //retval puntatore a struct.
}


/** dealloca l'area di gioco in tutte le sue parti al termine del gioco
 \param sea area da deallocare
 */
void distruggi_area (area_t * sea){
    //variabili locali
    area_t * mare=sea;
    if (mare==NULL) return;//gestione NULL
    int i;//variabile per il gioco
    char ** AREA=mare->a;
    int n=mare->N;
    for (i=0; i<n; i++)
        free(AREA[i]); //dealloco il puntatore di ogni elemento dell'array AREA (ovvero quello che punta alle singole righe)
    free(AREA); //libero il puntatore al primo elemento della prima colonna della matrice
    free(mare); //dealloco la struct.
}


//funzione della stampa dell'area di gioco modificata per permettere di visualizzare il numero di righe e colonne.
/*Sopra la prima riga viene visualizzato un numero intero (0,1,2...), sopra la prima colonna la lettera (A,B,C...).*/

void print_area_mia (area_t * sea){
    int i=0, j; //i posizione riga, j posizione colonna
    char b=65; //carattere ASCII corrispondente ad A.
    if ( sea == NULL ) return;//gestione NULL
    
    //caratteri stampati per scopi estetici: si vuole avere il numero di colonna sopra la colonna etc.
    
    printf("\n");
    printf("/");
    for(i=0;i<sea->M;i++) printf("%3i",i); //stampa i numeri sulla prima riga
    printf("\n ");
    for(i=0;i<sea->M;i++) printf("  -"); //reticolo parte superiore
    printf("\n");
    for (i=0; i<sea->N; i++) {
        printf("%c ",b++); //stampo carattere e numero
        printf("|"); //reticolo verticale
        for (j=0; j<sea->M; j++)
            printf("%c  ", sea->a[i][j]); //stampa effettiva della matrice come nel file fornito dai docenti
        printf("\n");
    }
    
    putchar('\n');
    printf("#navi: %d", sea->n_navi);//stampo il numero di navi presenti nella griglia
    putchar('\n');
    putchar('\n');
    
    return;
}

/*Funzione che stampa uno schema ausiliario utile nella partita. Sono presenti due griglie. La griglia di sinistra è uguale a quella stamapta in print_area_mia e rappresenta il proprio mare con le proprie navi. La griglia di destra mostra la situazione conosciuta sulla matrice dell'avversario, ovvero filtra la matrice dell'avversario mostrando solo i caratteri X, a, b e c. I parametri sono le due struc (umano e computer)t. I comandi per adattare la visualizzazione della griglia (numero colonna, spazi, confini della matrice...) sono gli stessi della funzione print_area_mia */

void schema(area_t*sea,area_t*computer){
    int i=0, j;
    char b=65;
    /*Comandi analoghi alla funzione print_area_mia*/
    printf("\n");
    if ( sea == NULL ) return;
    if(computer ==NULL) return;
    printf("La tua griglia:");
    for(i=0;i<sea->M-4+(sea->M-5)*2+7;i++)//Titolo sopra la griglia
        printf(" ");
    printf("La griglia del tuo avversario:\n");
    printf("\n");
    printf("/ ");
    for(i=0;i<sea->M;i++) printf("%2i ",i);
    printf("      ");
    printf("/ ");
    i=0;
    for(i=0;i<sea->M;i++) printf("%2i ",i);
    printf("\n ");
    for(i=0;i<sea->M;i++) printf("  -");
    printf("        ");
    for(i=0;i<sea->M;i++) printf("  -");
    printf("\n");
    for (i=0; i<sea->N; i++) {
        printf("%c ",b);
        printf("|");
        for (j=0; j<sea->M; j++) printf("%c  ", sea->a[i][j]);
        printf("     ");
        printf("%c |",b++);
        for (j=0; j<sea->M; j++){
            if(computer->a[i][j]==EMPTY_COLPITA||computer->a[i][j]==ONE_COLPITA||computer->a[i][j]==TWO_COLPITA||computer->a[i][j]==THREE_COLPITA)
                printf("%c  ", computer->a[i][j]); //filtro per mostrare solo le navi colpite.
            else printf(".  "); //in caso alternativo si mostra solo un punto.
        }
        
        printf("\n");
    }
    
    putchar('\n');
    //si indicano sotto le griglie le navi ancora a disposizione di ogni schieramento.
    printf("Hai ancora %d navi",sea->n_navi);
    for(i=0;i<sea->M-4+(sea->M-5)*2+5;i++)//situazione navi sotto la griglia giusta
        printf(" ");
    printf("Il computer ha ancora %d navi.", computer->n_navi);
    
    putchar('\n');//2 volte a capo per motivi estetici: questa funzione viene chiamata alla fase conclusiva del turno per visualizzare la situazione aggiornata delle navi al termine di un attacco
    
    putchar('\n');
    
    return;
}

/**Funzione che implementa le condizioni periodiche di bordo.
 \param i elemento della matrice
 \param n colonne d'ercole.
 \retval nuova posizione dell'elemento.
 */
int cond_board(int i, int n){
    if(i==-1) i=n-1;
    if(i==-2) i=n-2;
    if(i==-3) i=n-3;
    if(i==n) i=0;
    if(i==n+1) i=1;
    if(i==n+2) i=2;
    return i;
}


/*Funzione che verifica che sia effettivamente possibile inserire una nave di dimensione dim nella direzione dir in posizione i+1 j+1 sulla matrice della struct sea. Viene chiamata dopo che il giocatore o il computer hanno inserito/generato delle coordinate di inserimento di una nave, con annessa dimensione della nave e direzione di inserimento. Restituisce il valore 0 se l'inserimento nella data condizione è possibile oppure -1 se è impossibile. L'errore verrà poi gestito da altre funzioni che inviteranno a inserire delle nuove coordinate */
int verify_spec(int dim,int i,int j,area_t*sea,int dir){
    int k,h;//variabili di controllo
    int y=j,x=i; //posizioni
    area_t*mare=sea;
    if (mare==NULL) return -1;
    
    int n=mare->N,m=mare->M;
    int p,q; //p e q variabili di controllo ausiliarie
    char ** AREA=mare->a;
    switch(dir){
        case 0: //inserimento verso est: si controlla griglia di dimensioni 3x(dim+2)
            p=3;
            q=dim+2;
            break;
        case 1: //inserimento verso sud: si controlla griglia di dimensioni (dim+2)x3
            p=dim+2;
            q=3;
            break;
        default:
            return -1; //gestione errore in caso la direzione non sia valida
    }
    for(k=0;k<p;k++){ //cicli for che scorrono la griglia di dimensioni di cui sopra.
        x=i;
        for(h=0;h<q;h++)
        {
            if(x>=n||y>=m) return -1; //evito di inserire navi in griglie che non siano almeno 3x3
            if(AREA[x][y]!=EMPTY) return -1; //nel caso si incontra anche solo una casella non vuota si interrompe il ciclo e si ritorna il valore -1 (inserimento non possibile)
            x++;
            x=cond_board(x, n);
        }
        y++;
        y=cond_board(y, m);
    }
    return 0; //inserimento possibile
    
}
//funzione che verifica la presenza di uno slot 3x(dim+2) o (dim+2)x3 libero per inserire una nave. E' un controllo globale sulla matrice richiamando casella per casella la funzione verify spec e serve ad individuare la situazione per cui è impossibile completare l'inserimento delle navi richieste a causa della particolare disposizione generata/inserita, scongiurando il caso di loop in cui vengono continuamente richieste/generate nuove coordinate in una griglia in cui è impossibile inserire la nave richiesta.
int verify(area_t*sea,int dim){
    area_t*mare=sea;
    if (mare==NULL) return -1;
    int n=mare->N,m=mare->M;
    int h,k;
    for (h=0; h<n; h++){//cicli for annidati che scorrono tutta la matrice
        for (k=0;k<m; k++){
            if(verify_spec(dim,k,h,mare,0)==0) return 0; //appena si trova un possibile slot libero sia inserendo la nave verso est che verso sud si esce dalla funzione ritornando 0.
            if(verify_spec(dim,k,h,mare,1)==0) return 0;
        }
        
    }
    return -1;//inserimento non possibile: non esiste uno spazio di di dim 3x(dim+2) o (dim+2)x3 libero per inserire una nave.
}
/*funzione che inserisce una nave di dimensione dim nella griglia della struct sea. L'inserimento può essere automatico o manuale a seconda del parametro man, che viene preso in input nel corso del gioco sulla base della scelta dell'utente: man==1 inserimento manuale. man==0 automatico*/

int put_man(area_t*sea,int dim,int man)
{
    area_t*mare=sea;
    if (mare==NULL) return -1; //gestione del NULL prima di leggere i campi della struct, evitando errore di segmentazione.
    int n=mare->N,m=mare->M,navi=mare->n_navi,x=-1,y=-1,i,j,dir=-1,k;
    char c;
    char ** AREA=mare->a;
    char nave=EMPTY;
    switch(dim){ //assegnazione del char da inserire nella matrice a seconda della dimensione della nave
        case 1:
            nave=ONE;
            break;
        case 2:
            nave=TWO;
            break;
        case 3:
            nave=THREE;
            break;
    }
    
    /* Se la funzione verify restituisce 0 allora vuol dire che un posto per la nave si trova, quindi entro nel blocco while.  */
    if(verify(mare,dim)==0)
    {
        while(1)
        {
            if(man==1){ //inserimento manuale
                while(1){
                    printf("\nInserisci le coordinate (prima la riga, poi la colonna) ad es:B 5\n");
                    int rtn=scanf("%c %d",&c,&y); //parametro per gestione scanf
                    x=c-65;
                    if (rtn == 0) {    //inserimento non corretto
                        printf("\nDevi inserire dei numeri.\n");
                        empty_stdin(); // pulisce la stdin
                    }
                    else if (x<0||x>n||y<0||y>m) { //fuori dalla matrice
                        printf("\nLe tue navi devono rimanere sul mare!\n");
                        empty_stdin(); // pulisce la stdin
                    }
                    else {  /* input corretto */
                        empty_stdin(); // pulisce la stdin
                        break;
                    }
                }
                while(1){ //idem come sopra per la direzione della nave
                    printf("\nInserisci la direzione della nave \n0-sud \n1-est\n");
                    int rtn=scanf("%d",&dir);
                    if (rtn == 0) { // assegnamento della variabile errato
                        printf ("\nDevi inserire dei numeri.\n");
                        empty_stdin(); //pulisce la stdin
                    }
                    else if (dir!=0&&dir!=1) { //inserimento di un numero diverso da quelli richiesti
                        printf("\nNon hai inserito numeri validi!\n");
                        empty_stdin(); //pulisce la stdin
                    }
                    else {
                        empty_stdin(); //pulisce la stdin
                        break;
                    }
                }
                
            }
            if(man==0){//input automatico, genero numeri casuali. x e y coordinate su cui inserire la nave, dir è la direzione (0 sud, 1 est)
                x=rand()%n;
                y=rand()%m;
                dir=rand()%2;
            }
            i=x-1;//i e j posizioni della matrice da cui iniziare le operazioni di verifica (si verifica dalla riga e colonna precedente)
            j=y-1;
            i=cond_board(i, n);
            j=cond_board(j, m);
            if(verify_spec(dim, i, j, mare, dir)==0) //in caso sia effettivamente possibile inserire la nave nella casella di coordinate x y e orientamento tmp si avvia il ciclo coi parametri della dimensione della nave. dim è un int, mentre nave riprende le macro come indicato all'inizio di questa funzione
            {
                for(k=0;k<dim;k++){
                    AREA[x][y]=nave; // assegna la prima casella della nave
                    switch(dir)//imposta l'orientamento sulla base del parametro dir
                    {
                        case 0: // orientamento verticale
                            x++;
                            x=cond_board(x, n); // applico le condizioni al bordo
                            break;
                        case 1: // orientamento orizzontale
                            y++;
                            y=cond_board(y, m); //condizioni al bordo
                            break;
                    }
                }
                navi++;
                mare->n_navi=navi;//si aggiorna il numero di navi della struct
                return 0;//nav inserita
            }
            /* Se la condizione dell'if si è verificata allora la funzione restituisce il valore 0, altrimenti stampa un messaggio di errore (nel caso di inserimento manuale) e restituisce -1 */
            if(man==1) printf("\nNon puoi inserire una nave da %d qui, o in questa direzione.\n Ritenta.\n",dim); //se l'inserimento è manuale si stampa il messaggio di errore
        }
    }//se la funzione è arrivata a questo punto significa che non è possibile inserire navi di quella dimensione nella matrice. La funzione ritorna -1.
    printf("\nNon ci stanno così tante navi qua!! Ricominciamo.\n\n");
    return -1;
}
/*Funzioni put_ che inseriscono automaticamente le navi da uno, da due e da tre nella griglia della struct sea, utilizzando la funzione put_man*/
int put_one(area_t*sea){
    if (sea==NULL) return -1;  //gestione del NULL prima di leggere i campi della struct, evitando errore di segmentazione.
    return put_man(sea,1,0);
}

int put_two(area_t*sea){
    if (sea==NULL) return -1; // gestione NULL
    return put_man(sea,2,0);
}

int put_three(area_t*sea){
    if (sea==NULL) return -1; //gestione NULL
    return put_man(sea,3,0);
}

/*Funzione che gestisce l'interfaccia con l'utente per l'inserimento delle navi. La funzione inoltre va ad inserire nelle matrici di entrambe le struct il numero esatto di navi passato alla funzione. Prende in input le variabili scelta(inserimento manuale o automatico), n_navi ripettivamente per le navi da 1,2,3, le griglie del giocatore e del computer*/
int put(int scelta, unsigned n_navi1, unsigned n_navi2, unsigned n_navi3,area_t*umano,area_t*computer){
    if (umano==NULL||computer==NULL) return -1; // gestione NULL
    int i=0;
    while(1){
        init(umano);//inizializzo l'area di gioco in caso si sia entrati nuovamente nel blocco while al termine del ciclo
        for(i=0;i<n_navi3;i++) {//inserisco n_navi3 navi da 3.
            if(scelta==1) put_three(umano);//scelta disposizione automatica
            if(scelta==0){//scelta disposizione manuale
                if(verify(umano, 3)==-1){//verifico che ci sia posto nella griglia, se la verify restituisce -1 allora nella griglia non c'è più posto per inserire altre navi
                    printf("\n La tua disposizione di navi non va bene. Devi ricominciare da capo a disporle, oppure le puoi far disporre al computer.\n");
                    return -1;//se non c'è posto stampo il messaggio di errore e ritorno -1.
                }
                printf("\nInserisci la nave da tre (te ne mancano %d).\n",n_navi3-i);
                put_man(umano, 3,1);//inserisco la singola nave con la funzione put_man
                print_area_mia(umano);//visualizzo l'area per una migliore gestione dell'area di gioco
            }
        }//ripeto per le navi da 1 e da 2
        for(i=0;i<n_navi2;i++) {
            if(scelta==1) put_two(umano); //inserimento automatico
            if(scelta==0){ //inserimento manuale con controllo della disponibilità di spazio attraverso la funzione verify
                if(verify(umano, 2)==-1){
                    printf("\n La tua disposizione di navi non va bene. Devi ricominciare da capo a disporle, oppure le puoi far disporre al computer.\n");
                    return -1;
                }
                printf("\nInserisci la nave da due (te ne mancano %d.)\n",n_navi2-i);
                put_man(umano,2,1); //inserimento nave
                print_area_mia(umano); //stampa area
            }
        }
        for(i=0;i<n_navi1;i++) {
            if(scelta==1) put_one(umano); //inserimento automatico
            if(scelta==0){ //inserimento manuale con controllo della disponibilità di spazio attraverso la funzione verify
                if(verify(umano, 1)==-1){
                    printf("\n La tua disposizione di navi non va bene. Devi ricominciare da capo a disporle.\n");
                    return -1;
                }
                printf("\nInserisci la nave da uno (te ne mancano %d.)\n",n_navi1-i);
                put_man(umano,1,1); //inserimento nave
                print_area_mia(umano); //stampa area
            }
        }
        if (conta_navi(umano,THREE) == n_navi3&&conta_navi(umano, TWO)==n_navi2&&conta_navi(umano, ONE)==n_navi1) break;//controllo di averle inserite tutte. In caso contrario faccio ripartire il while. Il programma non cicla perché nella funzione inser_navi si verifica che sia possibile inserire quel numero di navi in quella griglia.
    }
    
    while(1){//inserisco tutte le navi per il computer
        init(computer); //inizializzo a zero la griglia del computer
        for(i=0;i<n_navi3;i++) put_three(computer);
        for(i=0;i<n_navi2;i++) put_two(computer);
        for(i=0;i<n_navi1;i++) put_one(computer);
        if (conta_navi(computer,THREE) == n_navi3&&conta_navi(computer, TWO)==n_navi2&&conta_navi(computer, ONE)==n_navi1) break;//inserite tutte?
    }
    return 0;//ritorno 0 se è andato tutto bene
}

int inser_navi(unsigned n_navi1, unsigned n_navi2, unsigned n_navi3, area_t*umano,area_t*computer){//gestisce l'interfaccia con l'utente perla scelta di inserimento delle navi(modo automatico o manuale) e chiama le funzioni di inserimento
    if (umano==NULL||computer==NULL) return -1; //gestione NULL
    int scelta;
    while(1){
        printf("\nVuoi inserire le navi manualmente o in modo randomico?\n0-Manualmente\n1-Sono pigro\n");
        int rtn=scanf("%d",&scelta);
        if (rtn == 0) {    /* gestione scanf, se rtn è 0 allora non vi sono stati lettura ed assegnamento corretti e si ripete il ciclo */
            printf ("\nDevi inserire dei numeri.\n");
            empty_stdin(); //pulisce la stdin
        }
        else if (scelta!=0&&scelta!=1) { //inserimento errato da parte dell'utente
            printf("\nNon è un opzione disponibile, mi dispiace. Premi 1 oppure 0 e poi ENTER.\n");
            empty_stdin();
        }
        else {  /* good input */
            empty_stdin();
            break;
        }
    }
    if(put(scelta, n_navi1, n_navi2, n_navi3, umano,computer)==-1) return -1;//chiama la funzione put per inserire le navi. Se l'inserimento non è andato a buon fine ritorna -1, altrimenti ritorna 0.
    return 0;
}


/*Funzione che regola le impostazioni di gioco (default/custom) e in caso i parametri (dimensioni griglia, num navi) all'apertura del gioco. Inoltre la funziona controlla, in caso di scelta manuale, che in una griglia di dimensione nxm possa effettivamente essere inserito il numero di navi desiderato dall'utente.*/

void custom(int*num_navi1,int*num_navi2,int*num_navi3, int*N,int*M){//i parametri della funzione, passati come puntatori saranno usati come valori di ritorno per inserirli nella funzione gioca.
    int imp=5,i=0,a=0,b=0,c=0,rtn=0;
    while(1){
        printf("\nVuoi giocare con le impostazioni di default o scegliere la dimensioni della griglia e quanti navi inserire?\n0-Default (griglia 8x8 con una nave da 1, due da 2, tre da 3)\n1-Custom\n");
        rtn=scanf("%d",&imp);
        if (rtn == 0) {    /* gestione scanf, se rtn è 0 allora non vi sono stati lettura ed assegnamento corretti e si ripete il ciclo */
            printf ("\nDevi inserire un numero.\n");
            empty_stdin(); // pulisce la stdin
        }
        else if (imp!=0&&imp!=1) { // inserimento valore errato
            printf("\nPer favore, inserisci solo i numeri richiesti.\n");
            empty_stdin();
        }
        else {  /* good input */
            empty_stdin();
            break;
        }
    }
    while(imp==1){//entro nel blocco while se voglio scegliere i parametri di gioco
        while(1){//ripeto il while annidato fino a quando non trovo parametri compatibili (ovvero le navi possono stare nella griglia)
            printf("\nScegli le dimensioni della griglia:\n");
            while(1)
            {
                printf("\nQuante righe deve avere il campo di gioco?\n");
                rtn=scanf("%d",N);
                if (rtn == 0) {    /* gestione scanf */
                    printf ("\nDevi inserire un numero.\n");
                    empty_stdin(); // pulisce la stdin
                }
                else if (*N>150) {
                    printf("\nNon ti sembra un po' grande?\n");
                    empty_stdin();
                }
                else {  /* good input */
                    empty_stdin();
                    break; //il ciclo si interrompe quando l'inserimento è andato a buon fine altrimenti chiede nuovamente il numero di righe
                }
                
            }
            while(1)
            {
                printf("\nQuante colonne deve avere il campo di gioco?\n"); //stessi comandi del caso precedente ma nel caso delle colonne della matrice
                rtn=scanf("%d",M);
                if (rtn == 0) {    /* gestione scanf */
                    printf ("\nDevi inserire un numero.\n");
                    empty_stdin();
                }
                else if (*M>150) {
                    printf("\nNon ti sembra un po' grande?\n");
                    empty_stdin();
                }
                else {  /* good input */
                    empty_stdin();
                    break;
                }
                
            }
            
            area_t*umano=crea_area(*N, *M);//creo un'area di gioco per testare se il numero delle navi e la dimensione della griglia sono compatibili.
            if (umano==NULL) return;
            while(1)
            {
                printf("\nQuante navi da uno?\n");
                rtn=scanf("%d",num_navi1);
                if(buffer(rtn)==0) break; //gestione scanf con la funzione buffer, il while si blocca in caso di errore
            }
            
            for(i=0;i<*num_navi1;i++) {//provo a inserire tutte le navi da uno
                if(put_one(umano)==-1) break; // il ciclo si interrompe in caso di errore
            }
            if((a=conta_navi(umano,ONE))!=*num_navi1) break; //se non le ho inserite tutte esco dal ciclo
            
            while(1)//ripeto per la navi da due e da tre in modo analogo
            {
                printf("\nQuante navi da due?\n");
                rtn=scanf("%d",num_navi2);
                if(buffer(rtn)==0) break;
            }
            for(i=0;i<*num_navi2;i++) {
                if(put_two(umano)==-1) break;
            }
            if((b=conta_navi(umano,TWO))!=*num_navi2) break;
            while(1)
            {
                printf("\nQuante navi da tre?\n");
                rtn=scanf("%d",num_navi3);
                if(buffer(rtn)==0) break;
            }
            for(i=0;i<*num_navi3;i++) {
                if(put_three(umano)==-1) break;
            }
            if((c=conta_navi(umano,THREE))!=*num_navi3) break;
            
            distruggi_area(umano);//dealloco l'area. Se ho finito ok, se qualcosa è andato storto devo crearla con nuovi parametri (diversa grandezza o numero di navi)
            if(a==*num_navi1&&b==*num_navi2&&c==*num_navi3){imp=0; break;}
        }
        //controllo se effettivamente le navi ci stanno, o se sono uscito dal blocco while senza averle inserite tutte. In caso positivo esco dal blocco while esterno, altrimenti inserisco nuovamente i parametri finché non trovo un match. Il messaggio di errore è gestito dalla funzione put_man.
        
    }
}



int affondata_tre(area_t*sea,int x,int y){//funzione che verifica se la nave da tre è affondata o solo colpita. In caso la nave sia affondata la funzione ritorna 0, altrimenti -1. E' distinta dai casi da 1 e 2 per la sua struttura leggermente più complessa
    area_t*mare=sea;
    if (mare==NULL) return -1;
    int n=mare->N, m=mare->M;
    char ** AREA=mare->a;
    int k;
    
    int i=x-2;
    int j=y-2;
    int I=i+1;//definisco due nuove variabili di controllo, utili per eseguire la verifica con un for.
    int J=j+1;
    for(k=0;k<3;k++,i++,j++,I++,J++){//si eseguono tre iterazioni, da -2 a +2, saltando la casella x y, in modo da verificare tutte le posizioni possibili occupate da una nave da 3
        i=cond_board(i, n);
        I=cond_board(I,n);
        j=cond_board(j,m);
        J=cond_board(J,m);
        if(i==x)i++;//non verifico la casella colpita x y
        if(j==y)j++;
        if(I==x)I++;
        if(J==y)J++;
        i=cond_board(i, n);
        I=cond_board(I,n);
        j=cond_board(j,m);
        J=cond_board(J,m);
        if(AREA[x][J]==THREE_COLPITA&&AREA[x][j]==THREE_COLPITA) return 0;//affinché la nave sia affondata devono essere colpite entrmbe le caselle adiacenti.
        if(AREA[I][y]==THREE_COLPITA&&AREA[i][y]==THREE_COLPITA) return 0;
    }
    return -1;
    
}

//funziona che determina l'esito di una mossa (acqua, colpito, affondato) analizzando la rispettiva area di gioco e aggiornando i campi della struct relativi alle statistiche di gioco, che verranno poi visualizzate a fine partita
int esegui_mossa(area_t * sea, int x, int y){
    area_t*mare=sea;
    if (mare==NULL) return -1;
    int n=mare->N, m=mare->M, navi=mare->n_navi,i=x-1,j,hit=mare->hit,miss=mare->misses;
    char ** AREA=mare->a;
    int h,k;
    
    switch(AREA[x][y]){//un solo caso possibile per l'acqua e per la nave da uno. Si aggiorna la macro e le variabili della struct.
        case EMPTY: // caso acqua
            AREA[x][y]=EMPTY_COLPITA;
            miss++; // si aggiornano le statistiche
            mare->misses=miss;
            return ACQUA;
        case ONE: // caso nave da 1
            AREA[x][y]=ONE_COLPITA;
            navi--; // si cambia il numero di navi
            mare->n_navi=navi;
            hit++; // aggiornamento statistiche
            mare->hit=hit;
            return AFFONDATA_ONE;
        case TWO://per la nave da due e da tre bisogna invece verificare se la nave è colpita o affondata. Per la nave da due si esegue un ciclo for controllando le caselle adiacienti, simile a quello della funzione verify.
            AREA[x][y]=TWO_COLPITA;
            for(k=0;k<3;k++){
                j=y-1;
                i=cond_board(i, n);
                for(h=0;h<3;h++){
                    j=cond_board(j, m);
                    if(AREA[i][j]==TWO){ //se nelle caselle adiaccenti trova altre caselle con la macro TWO significa che c'è una parte di nave ancora da colpire, e restituisce COLPITA_TWO
                        hit++;
                        mare->hit=hit;
                        return COLPITA_TWO;
                    }
                    j++;
                }
                i++;
            }
            /*se nelle caselle adiacenti non trova parti di nave ancora da colpire allora significa che la nave è stata affondata: si aggiornano numero di navi e statistiche e si restituisce AFFONDATA_TWO */
            navi--;
            mare->n_navi=navi;
            hit++;
            mare->hit=hit;
            return AFFONDATA_TWO;
            
        case THREE://per la nave da tre si segue una filosofia simile al caso TWO. Per la nave da tre si deve però verificare una condizione in più, ovvero devo controllare due caselle adiacenti. Questo viene svolto dalla funzione affondata_tre
            AREA[x][y]=THREE_COLPITA;
            if(affondata_tre(mare,x,y)==0){ //caso nave affondata, aggiorno numero di navi e statistiche e ritorno AFFONDATA_THREE
                navi--;
                mare->n_navi=navi;
                hit++;
                mare->hit=hit;
                return AFFONDATA_THREE;
            }
            if(affondata_tre(mare,x,y)==-1){ //caso nave colpita, aggiorno solo le statistiche poichè il numero di navi rimanenti è invariato e ritorno COLPITA_THREE
                hit++;
                mare->hit=hit;
                return COLPITA_THREE;
            }
            return ERRORE; // se non si verifica nessuno dei precedenti casi allora restituisco ERRORE
    }
    return ERRORE; // Se non si è verificato nessuno dei casi precedenti allora c'è un problema nella griglia e restituisco ERRORE
}

/*funzione di controllo per determinare la vittoria. non appena il numero di navi è zero ritorna 0.*/
int vittoria(area_t* sea)
{
    area_t*mare=sea;
    if (mare==NULL) return -1; //gestione NULL
    int navi=mare->n_navi;
    if(navi==0) return 0;
    return -1;
}

/*visualizzazione delle statistiche. Riporto i valori della struct, che sono aggiornati ad ogni turno sulla base delle mosse.*/

void stats(area_t*sea, area_t*sea2){
    if (sea==NULL) return; // GESTIONE NULL
    if (sea2==NULL) return;
    
    float ratio=(float)sea->hit/(sea2->shots); // calcola il rapporto tra attacchi che hanno dato come esito colpito o affondato e i tiri totali, per il giocatore e per il computer
    float ratio1=(float)sea2->hit/(sea->shots);
    
    // stampa tutti i parametri delle statistiche: colpiti, acqua, tiri totali, rapporto % tra colpiti e acqua
    printf ("+===================================================\n");
    printf ("|                     STATS                         \n");
    printf ("+---------------------------------------------------\n");
    printf ("|     TU   : %d colpite                             \n", sea2->hit);
    printf ("|            %d mancate                             \n", sea2->misses);
    printf ("|            %d tiri torali                         \n", sea->shots);
    printf ("|            %.2lf%% colpite/mancate                \n", ratio1*100);
    printf ("| COMPUTER : %d colpite                             \n", sea->hit);
    printf ("|            %d mancate                             \n", sea->misses);
    printf ("|            %d tiri totali                         \n", sea2->shots);
    printf ("|            %.2lf%% colpite/mancate                \n", ratio*100);
    printf ("+===================================================\n");
    
}

/*Funzione ausiliaria per la difficoltà intermedia (all'avvio del gioco verrà chiesto all'utente di selezionare la difficoltà desiderata). Il computer controlla che in caso abbia colpito una casella EMPTY nelle caselle adiacenti non ci siano pezzi di navi già colpite. In caso le navi colpite siano effettivamente presenti la funzione ritorna -1. Poichè nei dintorni di una nave non possono essere inserite altre navi la funzione scarta il tiro e ne genera un altro , di modo da colpire solo caselle in cui la disposizione di navi è permessa.
 \param x e y coordinate tiro
 \retval -1 in caso sono presenti navi colpite nelle caselle adiacenti*/
int intermedia(area_t*sea, int x, int y){
    area_t*mare=sea;
    if (mare==NULL) return -1;
    int n=mare->N, m=mare->M;
    char ** AREA=mare->a;
    int i=0,h,k;
    int j=0;
    if(AREA[x][y]==EMPTY){
        for (k=0;k<3;k++) {//controllo tutte le caselle adiacenti, alias la griglia 3x3 con al centro x y.
            j=y-1; //soliti parametri per effettuare il controllo
            i= x-1;
            i=cond_board(i, n);
            for(h=0;h<3;h++){
                j=cond_board(j, m);
                if(AREA[i][j]==ONE_COLPITA||AREA[i][j]==TWO_COLPITA||AREA[i][j]==THREE_COLPITA) return -1;
                j++; //condizione in cui sono presenti pezzi di nave
            }
            i++;
        }
    }//non si sono trovati pezzi di nave, quindi si ritorna 0 per indicare che è andato tutto bene.
    return 0;
}

//funzione ausiliaria per la modalità difficile. Verifica che, data una casella TWO_COLPITA o THREE_Colpito, nelle caselle adiacenti ci siano pezzi di nave già colpiti. In caso ci siano o la funzione non abbia in input TWO_Colpita o Three_colpita la funzione ritorna -1. In caso non ci siano e la casella sia effettivamente two_colpita e three_colpita la funzione ritorna 0. L'idea è quella di implementare un attacco che, una volta colpita una parte di nave, colpisca le caselle adiacenti fino all'affondamento nei turni successivi. Questo viene implementato successivamente nella funzione attacco_computer sulla base del valore restituito dalla funzione difficile //

int difficile(area_t*sea,int x,int y,int dim){
    area_t*mare=sea;
    int n=mare->N, m=mare->M;
    char ** AREA=mare->a;
    char b=EMPTY;
    if(dim==3) b=THREE_COLPITA; // assegno la stringa alla variabile b
    if(dim==2) b=TWO_COLPITA;
    int i=x+1;
    int j=y+1;
    // verifico le 4 caselle intorno alla casella fornita alla funzione: se una delle caselle intorno è già stata colpita in precedenza restituisco -1, altrimenti restituisco 0. In questo modo distinguerò il caso in cui ho gia trovato la direzione della nave (2 caselle allineate colpite) e quindi dovrò colpire una delle due caselle adiacenti alle due colpite, dal caso in cui non ho altre caselle colpite e quindi dovrò colpire in una delle 4 direzioni possibili intorno alla prima casella colpita
    if(AREA[x][y]==b){
        i=cond_board(i, n);
        if(AREA[i][y]==b) return -1;
        i=x-1;
        i=cond_board(i, n);
        if(AREA[i][y]==b) return -1;
        j=cond_board(j, m);
        if(AREA[x][j]==b) return -1;
        j=y-1;
        j=cond_board(j, m);
        if(AREA[x][j]==b) return -1;
        return 0;
    }
    return -1;
}


/*funzione che gestisce l'attacco del computer a seconda del livello selezionato.
 \retval e=esito (acqua,colpita_one etc. a seconda delle macro)*/

int attacco_computer(int livello, area_t*sea){
    area_t*mare=sea;
    if (mare==NULL) return -1;
    int J,k,h,I;
    int x,y,e=ERRORE,i=0,j=0;
    char c;
    int n=mare->N, m=mare->M;
    char ** AREA=mare->a;
    
    switch(livello){//livello facile: generazione casuale con rand
        case 0:
            x=rand()%n;
            y=rand()%m;
            e = esegui_mossa(mare,x,y); // esegue attacco
            c=x+65;
            if(e!=ERRORE) printf("Il computer ha colpito in ( %c, %d ):",c,y); // se non ci sono stati errori stampa le coordinate dell'attacco del pc
            return e;
        case 1: //livello intermedio: non si colpiscono caselle d'acqua che hanno intorno navi già colpite o affondate.
            x=rand()%n;
            y=rand()%m;
            if(intermedia(mare,x,y)==0) { //controlla che la casella generata in modo casuale rispetti il criterio e nel caso esegue l'attacco
                e=esegui_mossa(mare,x,y);
                c=x+65;
                if(e!=ERRORE) printf("Il computer ha colpito in ( %c, %d ):",c,y); // se non ci sono stati errori stampa le coordinate dell'attacco del pc
            }
            return e;
        case 2://livello difficile: si dà la priorità a finire di affondare una nave.
            for(x=0;x<n;x++)
            {
                for(y=0;y<m;y++)//for annidati per scorrere tutta la matrice
                {
                    if(difficile(mare,x,y,2)==0||difficile(mare, x, y, 3)==0) {//si scorre tutta la matrice cercando una casella two_colpita o three_colpita che non abbia altre navi colpite adiacenti, e dunque tale da restituire 0 con la funzione difficile. Quando si è trovata si genera un numero casuale che stabilire in quale casella adiacente colpire.
                        int tmp=rand()%4; // genera numero pseudocasuale tra per la direzione
                        switch(tmp){
                            case 0://sud
                                i=x+1;
                                j=y;
                                i=cond_board(i, n); // impone condizioni al bordo
                                break;
                            case 1://nord
                                i=x-1;
                                j=y;
                                i=cond_board(i, n);
                                break;
                            case 2://est
                                i=x;
                                j=y+1;
                                j=cond_board(j, m);
                                break;
                            case 3://ovest
                                i=x;
                                j=y-1;
                                j=cond_board(j, m);
                                break;
                        }
                        e=esegui_mossa(mare,i,j); // esegue attacco
                        c=i+65;
                        if(e!=ERRORE) printf("Il computer ha colpito in ( %c, %d ):",c,j); // se non ci sono stati errori stampa le coordinate dell'attacco del pc
                        return e;
                    }
                    if(AREA[x][y]==THREE_COLPITA&&affondata_tre(sea, x, y)==-1){//nel caso in cui non esista nella griglia una casella isolata three_colpita o two_colpita si arriva a questa parte di codice. Ora si deve controllare se la nave da tre è completamente affondata o se tentare di colpire la casella mancante. Entro nell'if in caso la nave non sia affondata.
                        i=x-1;
                        J=y+2;//variabile ausiliaria per evitare la verifica della casella y+2. Alla riga 879 il codice verifica effettivamente all'ultima iterazione del for più interno la casella y+2, poiché la j viene aggiornata. con la variabile J aggiro questo problema.
                        for(k=0;k<3;k++,i++){//cicli for annidati per trovare la casella adiacente colpita (si verifica il solito intorno 3x3)
                            j=y-1;
                            i=cond_board(i, n);
                            for(h=0;h<3;h++,j++){
                                j=cond_board(j, m);
                                if(i!=x||j!=y) //escludo la casella data, voglio trovare quella adiacente
                                    if(AREA[i][j]==THREE_COLPITA) break;
                            }
                            J=cond_board(J, m);
                            if(j==J) j--; // se coincidono allora abbasso j di uno
                            if(AREA[i][j]==THREE_COLPITA) break;
                        }
                        int tmp=rand()%2;//genero numero casuale per colpire nord/sud/ovest/est
                        I=i;
                        J=j;
                        switch(tmp){
                            case 0://varie casistiche a seconda della posizione della nave adiacente trovata. Ad es: se si è trovato che la nave adiacente si trova a est della nave di partenza, i valori di i e j saranno i=x e j=y+1. Ovvero l'x è fissato e si colpisce variando solo la y. A seconda della direzione da colpire si assegnerà un nuovo valore a j, la coordinata di lancio, colpendo o a destra della nave trovata (case 0:j=y+2) o a sinistra (case 1:j=y-1;)
                                I=x+1;
                                I=cond_board(I, n);
                                if(i==I) i=x+2;
                                I=x-1;
                                I=cond_board(I, n);
                                if(i==I) i=x+1;
                                J=y+1;
                                J=cond_board(J, m);
                                if(j==J) j=y+2;
                                J=y-1;
                                J=cond_board(J, m);
                                if(j==J) j=y+1;
                                break;
                            case 1:
                                I=x-1;
                                I=cond_board(I, n);
                                if(i==I) i=x-2;
                                I=x+1;
                                I=cond_board(I, n);
                                if(i==I) i=x-1;
                                J=y-1;
                                J=cond_board(J, m);
                                if(j==J) j=y-2;
                                J=y+1;
                                J=cond_board(J, m);
                                if(j==J) j=y-1;
                        }
                        i=cond_board(i, n);
                        j=cond_board(j, m);
                        e=esegui_mossa(mare, i, j); //esegue l'attacco
                        c=i+65;
                        if(e!=ERRORE) printf("Il computer ha colpito in ( %c, %d ):",c,j); //stampa le coordinate dell'attacco del pc
                        return e;
                    }
                }
            }
            while(1){//Nel caso in cui non si possa effettuare l'attacco intelligente in quanto non ci sono navi colpite che devono essere affondate si provvede alla generazione di numeri pseudo-casuali, richiamando la funzione intermedia.
                x=rand()%n;
                y=rand()%m;
                if(intermedia(mare,x,y)==0){
                    e=esegui_mossa(mare,x,y);
                    c=x+65;
                    if(e!=ERRORE) printf("Il computer ha colpito in ( %c, %d ):",c,y);
                    
                }
                return e;
            }
            
    }
    e=ERRORE; // attacco non andato a buon fine: nella funzione di gioco verrà ripetuta nuovamente la funzione finchè non si restituisce un valore diverso
    return e;
}

int diffi(){//funzione per scegliere il livello di difficoltà
    int diff;
    while(1){
        printf("\nSeleziona difficoltà:\n0-Facile\n1-Intermedio\n2-Difficile\n");
        int rtn=scanf("%d",&diff);
        if (rtn == 0) {    /* controllo della scanf */
            printf ("\nDevi inserire dei numeri.\n");
            empty_stdin();
        }
        else if (diff!=0&&diff!=1&&diff!=2) {
            printf("\nSembra che la tua scelta non sia contemplata dalle opzioni...\n");
            empty_stdin();
        }
        else {  /* good input */
            empty_stdin();
            break;
        }
    }
    return diff;
}



/*Funzione per decidere chi gioca per primo*/
int primo(){
    int primo,testa;
    while(1){
        printf("\nChi gioca per primo?\n0-Scelta manuale\n1-Testa o croce\n");
        int rtn=scanf("%d",&primo);
        if (rtn == 0) {    /* matching failure */
            printf ("\nDevi inserire dei numeri.\n");
            empty_stdin();
        }
        else if (primo!=0&&primo!=1) {
            printf("\nSembra che la tua scelta non sia contemplata dalle opzioni...\n");
            empty_stdin();
        }
        else {  /* good input */
            empty_stdin();
            break;
        }
    }
    if(primo==0)//scelta manuale
        while(1){
            printf("\n0-Tu\n1-Computer\n");
            int rtn=scanf("%d",&testa);//gestione scanf
            if (rtn == 0) {    /* matching failure */
                printf ("\nDevi inserire dei numeri.\n");
                empty_stdin();
            }
            else if (testa!=0&&testa!=1) {
                printf("\nSembra che la tua scelta non sia contemplata dalle opzioni...\n");
                empty_stdin();
            }
            else {  /* good input */
                empty_stdin();
                putchar('\n');
                putchar('\n');
                if(testa==1) return 0;//inizia il computer:la funzione ritorna 0 per non entrare nel primo while della funzione gioca
                return ERRORE;//inizia il giocatore: la funzione ritorna ERRORE per entrare nel blocco while della funzione gioca
            }
        }
    else//scelta testa o croce
    {
        while(1){
            printf("\nScegli la tua fortuna!\n0-Testa\n1-Croce\n");
            int rtn=scanf("%d",&testa);
            if (rtn == 0) {    /* matching failure */
                printf ("\nDevi inserire dei numeri.\n");
                empty_stdin();
            }
            else if (testa!=0&&testa!=1) {
                printf("\nSembra che la tua scelta non sia contemplata dalle opzioni...\n");
                empty_stdin();
            }
            else {  /* good input */
                empty_stdin();
                break;
            }
        }
        int tmp=rand()%2;//generazione numeri casuali
        if(tmp==0) printf("\nE' uscita testa!\n");
        if(tmp==1) printf("\nE' uscita croce!\n");
        if(tmp==testa) {
            printf("\nInizi tu!\n");
            return ERRORE;//inizia il giocatore: la funzione ritorna ERRORE per entrare nel blocco while della funzione gioca
        }
        else
        {
            printf("\nIl computer gioca per primo.\n\n\n");
            return 0;//inizia il computer:la funzione ritorna 0 per non entrare nel primo while della funzione gioca
        }
    }
}

void gioca(unsigned n, unsigned m, unsigned n_navi1, unsigned n_navi2, unsigned n_navi3){//funzione che gestisce il gioco
    area_t*umano;
    area_t*computer;
    int y=0,e=ERRORE,diff;
    char x=0;
    umano=crea_area(n,m);//alloco e inizializzo le aree.
    computer=crea_area(n,m);
    if (umano==NULL) return; // gestione NULL
    if (computer==NULL) return;
    while (inser_navi(n_navi1, n_navi2, n_navi3, umano, computer)==-1) { //inserimento delle navi, se non avviene in modo corretto si reinizializzano le griglie e si rientra nel cico
        init(umano);
        init(computer);
    }
        diff=diffi(); //impostazione livello di difficoltà
        e=primo();
        while(1){//blocco partita
            while(e==ERRORE)//turno del giocatore
            {
                while(1){
                    printf("\nInserisci una mossa (LETTERA MAIUSCOLA numero invio) ad es. B 5\n");
                    int rtn=scanf("%c %d",&x,&y);// gestone scanf
                    x-=65;
                    if (rtn == 0) {     //matching failure
                        printf ("\nInserimento non valido.\n");
                        empty_stdin();
                    }
                    else if (x<0||x>n-1||y<0||y>m-1) { //coordinate scorrette
                        printf("\nStai cercando di colpire la terraferma? Rimani nel mare!\n");
                        empty_stdin();
                    }
                    else {
                        empty_stdin();
                        break;
                    }
                }
                 e = esegui_mossa(computer,x,y); //effettua l'attacco nelle coordinate inserite
                x+=65;
                if(e!=0) printf("Hai colpito in ( %c, %d ):",x,y); // stampa il risultato
                if(e==ERRORE) printf("\nQualcosa non va! Sei sicuro di non aver già colpito questa casella?\n Ritenta.\n");
            }//impossibile colpire caselle già colpite. Nel caso la funzione esegui_mossa ritorna Errore e il blocco while si ripete.
            if(e!=0) stampa_esito (e); // stampa il risultato in standard output
            umano->shots++;//aggiorno il numero di tiri
            if(vittoria(computer)==0){//verifico che il numero di navi del computer sia uguale a 0.
                printf("\n\n\n\n\n\n\n\n\n\n\n                       Hai vinto!\n");
                stats(umano,computer);//printo le statistiche
                //Il disegno YOU WIN è di sagnew https://github.com/sagnew/Battleship
                printf("               Y88b    /   ,88~-_   888   |       Y88b         / 888  888b    |  d8b  \n");
                printf("                Y88b  /   d888   \\  888   |        Y88b       /  888  |Y88b   | !Y88! \n");
                printf("                 Y88b/   88888    | 888   |         Y88b  e  /   888  | Y88b  |  Y8Y  \n");
                printf("                  Y8Y    88888    | 888   |          Y88bd8b/    888  |  Y88b |   8   \n");
                printf("                   Y      Y888   /  Y88   |           Y88Y8Y     888  |   Y88b|   e   \n");
                printf("                  /        `88_-~    \\8__/             Y  Y      888  |    Y888  (8)  \n");
                distruggi_area(computer);//dealloco le aree
                distruggi_area(umano);
                return;//fine gioco
            }
            e=ERRORE;//TURNO COMPUTER
            while(e==ERRORE) e=attacco_computer(diff, umano);//chiamo la funzione per l'attacco del computer e attacco finché non colpisco una casella libera
            stampa_esito (e);
            computer->shots++;//aggiorno tiri
            schema(umano, computer);//stampo lo schema di gioco
            if(vittoria(umano)==0){//verifico che le navi dell'umano sia 0
                printf("\n\n\n\n\n\n\n\n\n\n\n                       Hai perso!\n");
                stats(umano,computer);
                distruggi_area(computer);
                distruggi_area(umano);
                return;//fine gioco
            }
            e=ERRORE;
        }
    }
    
