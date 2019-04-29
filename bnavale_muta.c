//
//  bnavale_muta.c
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


void empty_stdin (void) /* Funzionare per vuotare la stdin */
{
    int c = getchar();
    
    while (c != '\n' && c != EOF)
        c = getchar();
}

int buffer (int rtn){/*Gestione buffer*/
    if (rtn == 0) {    /* matching failure */
        fputs ("\nDevi inserire un numero.\n", stderr);
        empty_stdin();
        return -1;
    }
    else {  /* good input */
        empty_stdin();
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
    mare->n_navi=0;
}

/*Funzione che crea e inizializza una struct sea. Si veda navale.h per dettagli.  */

area_t * crea_area (unsigned n, unsigned m){
    
    int i;
    char ** AREA;
    
    area_t * mare=NULL; //inizializzo il puntatore a NULL.
    mare = (area_t*)malloc(sizeof(area_t)); //alloco il puntatore alla struct.
    AREA=malloc((n)*sizeof(char * )); //alloco la prima colonna della matrice AREA come un array di puntatori a char di lunghezza n.
    if (mare==NULL||AREA==NULL) return NULL;
    //inizializzazione dei campi della struct. Si veda bnavale.h per dettagli.
    mare->N=n;
    mare->M=m;
    mare->a=AREA;
    mare->hit=0;
    mare->misses=0;
    mare->shots=0;
    
    for (i=0; i<n; i++) {
        AREA[i]=malloc((m)*sizeof(char));
        if(AREA[i]==NULL) return NULL;
    } //alloco tutti gli array puntati da AREA come array di lunghezza m contenti caratteri.
    
    init(mare); //inizializzo la matrice ad EMPTY
    
    return mare; //retval puntatore a struct.
}


/** dealloca l'area di gioco in tutte le sue parti
 \param sea area da deallocare
 */
void distruggi_area (area_t * sea){
    //variabili locali
    area_t * mare=sea;
    if (mare==NULL) return;
    int i;
    char ** AREA=mare->a;
    int n=mare->N;
    for (i=0; i<n; i++)
        free(AREA[i]); //dealloco il puntatore di ogni elemento dell'array AREA (ovvero quello che punta alle singole righe)
    free(AREA); //libero il puntatore al primo elemento della prima colonna della matrice
    free(mare); //dealloco la struct.
}


//area di gioco modificata per permettere di visualizzare il numero di righe e colonne.
/*Sopra la prima riga viene visualizzato un numero intero (0,1,2...), sopra la prima colonna sia la lettera che il numero (A0,B1,C2...). L'idea iniziale era di avere sulle righe solo la lettera. Questa soluzione non convince nel caso in cui il numero delle righe superi quello delle lettere dell'alfabeto. Cercando una soluzione che potesse coinciliare sia l'aspetto grafico che l'interfaccia col giocatore, si è optato per tenere sia lettera che numero, chiedendo di inserire il numero accanto alla lettera.*/
/**condizioni periodiche di bordo.
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


/*Funzione che verifica che sia effettivamente possibile inserire una nave di dimensione dim nella direzione dir in posizione i+1 j+1 sulla matrice della struct sea.*/
int verify_spec(int dim,int i,int j,area_t*sea,int dir){
    int k,h;//variabili di controllo
    int y=j,x=i; //posizioni
    area_t*mare=sea;
    if (mare==NULL) return -1;
    
    int n=mare->N,m=mare->M,p,q;
    char ** AREA=mare->a;
    switch(dir){
        case 0: //inserimento verso est: si controlla griglia di dimensioni 3xdim+2
            p=3;
            q=dim+2;
            break;
        case 1: //inserimento verso sud: si controlla griglia di dimensioni dim+2x3
            p=dim+2;
            q=3;
            break;
        default:
            return -1;
    }
    for(k=0;k<p;k++){ //cicli for che scorrono la griglia di dimensioni di cui sopra.
        x=i;
        for(h=0;h<q;h++)
        {
            if(AREA[x][y]!=EMPTY) return -1; //nel caso si incontra anche solo una casella non vuota si interrompe il ciclo e si ritorna il valore -1 (inserimento non possibile)
            x++;
            x=cond_board(x, n);
        }
        y++;
        y=cond_board(y, m);
    }
    return 0; //inserimento possibile
    
}
//funzione che verifica la presenza di uno slot 3x(dim+2) o (dim+2)x3 libero per inserire una nave.
int verify(area_t*sea,int dim){
    area_t*mare=sea;
    if (mare==NULL) return -1;
    int n=mare->N,m=mare->M,h,k;
    for (h=0; h<n; h++){
        for (k=0;k<m; k++){
            if(verify_spec(dim,k,h,mare,0)==0) return 0; //appena si trova un possibile slot libero sia inserendo la nave verso est che verso sud si esce dalla funzione ritornando 0.
            if(verify_spec(dim,k,h,mare,1)==0) return 0;
        }
        
    }
    return -1;
}
/*funzione che inserisce una nave di dimensione dim nella griglia della struct sea. L'inserimento può essere automatico o manuale a seconda del parametro man. man==1 inserimento manuale. man==0 automatico*/

int put_man(area_t*sea,int dim,int man)
{
    area_t*mare=sea;
    if (mare==NULL) return -1;
    int n=mare->N,m=mare->M,navi=mare->n_navi,x=-1,y=-1,i,j,tmp=-1,k;
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
    
    /* Se la funzione verify restituisce 0 allora vuol dire che un posto per la nave si trova.  */
    if(verify(mare,dim)==0)
    {
        while(1)
        {
            if(man==1){ //inserimento manuale
                for(;;){
                    printf("\nInserisci le coordinate (prima la riga, poi la colonna):\n");
                    int rtn=scanf("%d %d",&x,&y); //parametro per gestione scanf
                    if (rtn == 0) {    //condizione false
                        printf("\nDevi inserire dei numeri.\n");
                        empty_stdin();
                    }
                    else if (x<0||x>n||y<0||y>m) { //fuori dalla matrice
                        printf("\nLe tue navi devono rimanere sul mare!\n");
                        empty_stdin();
                    }
                    else {  /* input corretto */
                        empty_stdin();
                        break;
                    }
                }
                for(;;){ //idem come sopra per la direzione della nave
                    printf("\nInserisci la direzione della nave \n0-sud \n1-est\n");
                    int rtn=scanf("%d",&tmp);
                    if (rtn == 0) {
                        fputs ("\nDevi inserire dei numeri.\n", stderr);
                        empty_stdin();
                    }
                    else if (tmp!=0&&tmp!=1) {
                        printf("\nNon hai inserito numeri validi!\n");
                        empty_stdin();
                    }
                    else {
                        empty_stdin();
                        break;
                    }
                }
                
            }
            if(man==0){//input automatico, genero numeri casuali. x e y coordinate su cui inserire la nave, tmp è la direzione (0 sud, 1 est)
                x=rand()%n;
                y=rand()%m;
                tmp=rand()%2;
            }
            i=x-1;//i e j posizioni della matrice da cui iniziare le operazioni di verifica (si verifica dalla riga e colonna precedente)
            j=y-1;
            i=cond_board(i, n);
            j=cond_board(j, m);
            if(verify_spec(dim, i, j, mare, tmp)==0) //in caso sia effettivamente possibile inserire la nave nella casella di coordinate x y e orientamento tmp si avvia il ciclo coi parametri della dimensione della nave. dim è un int, mentre nave riprende le macro come indicato all'inizio di questa funzione
            {
                for(k=0;k<dim;k++){
                    AREA[x][y]=nave;
                    switch(tmp)//orientamento
                    {
                        case 0:
                            x++;
                            x=cond_board(x, n);
                            break;
                        case 1:
                            y++;
                            y=cond_board(y, m);
                            break;
                    }
                }
                navi++;
                mare->n_navi=navi;//si aggiorna il numero di navi della struct
                return 0;
            }
            if(man==1) printf("\nNon puoi inserire una nave da %d qui, o in questa direzione.\n Ritenta.\n",dim); //se l'inserimento è manuale si stampa il messaggio di errore
        }
    }//se la funzione è arrivata a questo punto significa che non è possibile inserire navi di quella dimensione nella matrice. La funzione ritorna -1.
    if(man==1) printf("\nNon ci stanno delle navi qua!!\n\n");
    return -1;
}
/*Funzioni put_ che inseriscono automaticamente le navi da uno, da due e da tre nella griglia della struct area_t*/
int put_one(area_t*sea){
    if (sea==NULL) return -1;
    return put_man(sea,1,0);
}

int put_two(area_t*sea){
    if (sea==NULL) return -1;
    return put_man(sea,2,0);
}

int put_three(area_t*sea){
    if (sea==NULL) return -1;
    return put_man(sea,3,0);
}

/*Funzione che gestisce l'interfaccia con l'utente per l'inserimento delle navi. La funzione inoltre va ad inserire nelle matrici di entrambe le struct il numero esatto di navi passato alla funzione.*/
int put(int scelta, unsigned n_navi1, unsigned n_navi2, unsigned n_navi3,area_t*umano,area_t*computer){
    if (umano==NULL||computer==NULL) return -1;
    int i=0;
    while(1){
        init(umano);
        for(i=0;i<n_navi3;i++) {//inserisco n_navi3 navi da 3.
            if(scelta==1) put_three(umano);//scelta disposizione automatica
            if(scelta==0){//scelta inserimento manuale
                if(verify(umano, 3)==-1){//verifico che ci sia posto nella griglia
                    printf("\n La tua disposizione di navi non va bene. Eppure ce le puoi far stare! Ti do la possibilità di scegliere nuovamente se metterle da solo o se farlo fare al computer.\n");
                    return -1;//se non c'è posto stampo il messaggio di errore e ritorno -1.
                }
                printf("\nInserisci la nave da tre (te ne mancano %d).\n",n_navi3-i);
                put_man(umano, 3,1);//inserisco la singola nave con la funzione put_man
                //print_area_mia(umano);//visualizzo l'area per una migliore gestione dell'area di gioco
            }
        }//ripeto per le navi da 1 e da 2
        for(i=0;i<n_navi2;i++) {
            if(scelta==1) put_two(umano);
            if(scelta==0){
                if(verify(umano, 3)==-1){
                    printf("\n La tua disposizione di navi non va bene. Eppure ce le puoi far stare in questa griglia! Ti do la possibilità di scegliere nuovamente se metterle da solo o se farlo fare al computer.\n");
                    return -1;
                }
                printf("\nInserisci la nave da due (te ne mancano %d.)\n",n_navi2-i);
                put_man(umano,2,1);
                //print_area_mia(umano);
            }
        }
        for(i=0;i<n_navi1;i++) {
            if(scelta==1) put_one(umano);
            if(scelta==0){
                if(verify(umano, 3)==-1){
                    printf("\n La tua disposizione di navi non va bene. Eppure ce le puoi far stare! Ti do la possibilità di scegliere nuovamente se metterle da solo o se farlo fare al computer.\n");
                    return -1;
                }
                printf("Inserisci la nave da uno (te ne mancano %d.)\n",n_navi1-i);
                put_man(umano,1,1);
                //print_area_mia(umano);
            }
        }
        if (conta_navi(umano,THREE) == n_navi3&&conta_navi(umano, TWO)==n_navi2&&conta_navi(umano, ONE)==n_navi1) break;//controllo di averle inserite tutte. In caso contrario faccio ripartire il while. Il programma non cicla perché nella funzione inser_navi si verifica che sia possibile inserire quel numero di navi in quella griglia.
    }
    
    while(1){//inserisco tutte le navi per il computer
        init(computer);
        for(i=0;i<n_navi3;i++) put_three(computer);
        for(i=0;i<n_navi2;i++) put_two(computer);
        for(i=0;i<n_navi1;i++) put_one(computer);
        if (conta_navi(computer,THREE) == n_navi3&&conta_navi(computer, TWO)==n_navi2&&conta_navi(computer, ONE)==n_navi1) break;//inserite tutte?
    }
    return 0;//ritorno 0 se è andato tutto bene
}

/*Funzione che regola le impostazioni di gioco (default/custom) e in caso i parametri (dimensioni griglia, num navi). Inoltre la funziona controlla, in caso di scelta manuale, che in una griglia di dimensione nxm possa effettivamente essere inserito il numero di navi desiderato dall'utente.*/



int affondata_tre(area_t*sea,int x,int y){//funzione che verifica se la nave da tre è affondata o solo colpita. In caso la nave sia affondata la funzione ritorna 0, altrimenti -1.
    area_t*mare=sea;
    if (mare==NULL) return -1;
    int n=mare->N, m=mare->M;
    char ** AREA=mare->a;
    int k;
    
    int i=x-2;
    int j=y-2;
    int I=i+1;//definisco due nuove variabili di controllo, utili per eseguire la verifica con un for.
    int J=j+1;
    for(k=0;k<3;k++,i++,j++,I++,J++){//si eseguono tre iterazioni, da -2 a +2, saltando la casella x y.
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

//funziona che determina l'esito di una mossa (acqua, colpito, affondato)
int esegui_mossa(area_t * sea, int x, int y){
    area_t*mare=sea;
    if (mare==NULL) return -1;
    int n=mare->N, m=mare->M, navi=mare->n_navi,i=x-1,j,hit=mare->hit,miss=mare->misses;
    char ** AREA=mare->a;
    int h,k;
    
    switch(AREA[x][y]){//un solo caso possibile per l'acqua e per la nave da uno. Si aggiorna la macro e le variabili della struct.
        case EMPTY:
            AREA[x][y]=EMPTY_COLPITA;
            miss++;
            mare->misses=miss;
            return ACQUA;
        case ONE:
            AREA[x][y]=ONE_COLPITA;
            navi--;
            mare->n_navi=navi;
            hit++;
            mare->hit=hit;
            return AFFONDATA_ONE;
        case TWO://per la nave da due e da tre bisogna invece verificare se la nave è colpita o affondata. Per la nave da due si esegue un ciclo for controllando le caselle adiacienti, simile a quello della funzione verify.
            AREA[x][y]=TWO_COLPITA;
            for(k=0;k<3;k++){
                j=y-1;
                i=cond_board(i, n);
                for(h=0;h<3;h++){
                    j=cond_board(j, m);
                    if(AREA[i][j]==TWO){
                        hit++;
                        mare->hit=hit;
                        return COLPITA_TWO;
                    }
                    j++;
                }
                i++;
            }
            navi--;
            mare->n_navi=navi;
            hit++;
            mare->hit=hit;
            return AFFONDATA_TWO;
            
        case THREE://per la nave da tre si segue una filosofia simile al caso TWO. Per la nave da tre si deve però verificare una condizione in più, ovvero devo controllare due caselle adiacienti.
            AREA[x][y]=THREE_COLPITA;
            if(affondata_tre(mare,x,y)==0){
                navi--;
                mare->n_navi=navi;
                hit++;
                mare->hit=hit;
                return AFFONDATA_THREE;
            }
            if(affondata_tre(mare,x,y)==-1){
                hit++;
                mare->hit=hit;
                return COLPITA_THREE;
            }
            return ERRORE;
    }
    return ERRORE;
}

/*funzione di controllo per determinare la vittoria. non appena il numero di navi è zero ritorna 0.*/
int vittoria(area_t* sea)
{
    area_t*mare=sea;
    if (mare==NULL) return -1;
    int navi=mare->n_navi;
    if(navi==0) return 0;
    return -1;
}

/*Funzione ausiliaria per la difficoltà intermedia. Il computer controlla che in caso abbia colpito una casella EMPTY nelle caselle adiacenti non ci siano pezzi di navi già colpite. In caso le navi colpite siano effettivamente presenti la funzione ritorna -1.
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

//funzione ausiliaria per la modalità difficile. Verifica che, data una casella TWO_COLPITA o THREE_Colpito, nelle caselle adiacienti non ci siano pezzi di nave già colpiti. In caso ci siano o la funzione non abbia in input TWO_Colpita o Three_colpita la funzione ritorna -1. In caso non ci siano e la casella sia effettivamente two_colpita e three_colpita la funzione ritorna 0.

int difficile(area_t*sea,int x,int y,int dim){
    area_t*mare=sea;
    int n=mare->N, m=mare->M;
    char ** AREA=mare->a;
    char a=EMPTY;
    if(dim==3) a=THREE_COLPITA;
    if(dim==2) a=TWO_COLPITA;
    int i=x+1;
    int j=y+1;
    if(AREA[x][y]==a){
        i=cond_board(i, n);
        if(AREA[i][y]==a) return -1;
        i=x-1;
        i=cond_board(i, n);
        if(AREA[i][y]==a) return -1;
        j=cond_board(j, m);
        if(AREA[x][j]==a) return -1;
        j=y-1;
        j=cond_board(j, m);
        if(AREA[x][j]==a) return -1;
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
    int n=mare->N, m=mare->M;
    char ** AREA=mare->a;
    
    switch(livello){//livello facile: generazione casuale
        case 0:
            x=rand()%n;
            y=rand()%m;
            e = esegui_mossa(mare,x,y);
            //if(e!=ERRORE) printf("Il computer ha colpito in ( %d, %d ):",x,y);
            return e;
        case 1: //livello intermedio: non si colpiscono caselle d'acqua che hanno intorno navi già colpite o affondate.
            x=rand()%n;
            y=rand()%m;
            if(intermedia(mare,x,y)==0) {
                e=esegui_mossa(mare,x,y);
                //if(e!=ERRORE) printf("Il computer ha colpito in ( %d, %d ):",x,y);
            }
            return e;
        case 2://livello difficile: si dà la priorità a finire di affondare una nave.
            for(x=0;x<n;x++)
            {
                for(y=0;y<m;y++)
                {
                    if(difficile(mare,x,y,2)==0||difficile(mare, x, y, 3)==0) {//si scorre tutta la matrice cercando una casella two_colpita o three_colpita che non abbia altre navi colpite adiacenti. Quando si è trovata si genera un numero casuale che stabilire in quale casella adiacente colpire.
                        int tmp=rand()%4;
                        switch(tmp){
                            case 0://sud
                                i=x+1;
                                j=y;
                                i=cond_board(i, n);
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
                        e=esegui_mossa(mare,i,j);
                        //if(e!=ERRORE) printf("Il computer ha colpito in ( %d, %d ):",x,y);
                        return e;
                    }
                    if(AREA[x][y]==THREE_COLPITA&&affondata_tre(sea, x, y)==-1){//nel caso in cui non esista nella griglia una casella isolata three_colpita o two_colpita si arriva a questa parte di codice. Ora si deve controllare se la nave da tre è completamente affondata o se si tentare di colpire la casella mancante
                        i=x-1;
                        J=y+2;//variabile ausiliaria per evitare la verifica della casella y+2. Alla riga 818 il codice verifica effettivamente all'ultima iterazione del for più interno la casella y+2, poiché la j viene aggiornata. con la variabile J aggiro questo problema.
                        for(k=0;k<3;k++,i++){//cicli for annidati per trovare la casella adiacente colpita (si verifica il solito intorno 3x3)
                            j=y-1;
                            i=cond_board(i, n);
                            for(h=0;h<3;h++,j++){
                                j=cond_board(j, m);
                                if(i!=x||j!=y) //escludo la casella data, voglio trovare quella adiacente
                                    if(AREA[i][j]==THREE_COLPITA) break;
                            }
                            J=cond_board(J, m);
                            if(j==J) j--;
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
                        e=esegui_mossa(mare, i, j);
                       // if(e!=ERRORE) printf("Il computer ha colpito in ( %d, %d ):",x,y);
                        return e;
                    }
                }
            }
            while(1){//Nel caso in cui non si possa effettuare l'attacco intelligente si provvede alla generazione di numeri pseudo-casuali.
                x=rand()%n;
                y=rand()%m;
                if(intermedia(mare,x,y)==0){
                    e=esegui_mossa(mare,x,y);
                  //  if(e!=ERRORE) printf("Il computer ha colpito in ( %d, %d ):",x,y);
                    
                }
                return e;
            }
            
    }
    e=ERRORE;
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




int gioca1(unsigned n, unsigned m, unsigned n_navi1, unsigned n_navi2, unsigned n_navi3, int diff){//funzione che gestisce un'intera partita
    area_t*umano;
    area_t*computer;
    int e=ERRORE;
    umano=crea_area(n,m);//alloco e inizializzo le aree.
    computer=crea_area(n,m);
    if (umano==NULL) return EXIT_FAILURE;
    if (computer==NULL) return EXIT_FAILURE;
    while (put(1,n_navi1, n_navi2, n_navi3, umano, computer)==-1) init(umano); //inserisco le navi. Finché l'insermento non va a buon fine ripeto inizializzando l'area di gioco.
    while(1){//blocco partita
        while(e==ERRORE)//turno UMANO
        {
            e = attacco_computer(diff, computer);
        }//impossibile colpire caselle già colpite. Nel caso la funzione esegui_mossa ritorna Errore e il blocco while si ripete.
        umano->shots++;//aggiorno il numero di tiri
        if(vittoria(computer)==0){
            //verifico che il numero di navi del computer sia uguale a 0.
            distruggi_area(computer);//dealloco le aree
            distruggi_area(umano);
            return 0;//ritorno 0 se vince umano
        }
        e=ERRORE;//TURNO COMPUTER
        while(e==ERRORE) e=attacco_computer(diff, umano);//chiamo la funzione per l'attacco del computer e attacco finché non colpisco una casella libera
        computer->shots++;//aggiorno tiri
        //stampo lo schema di gioco
        if(vittoria(umano)==0){//verifico che le navi dell'umano sia 0
            distruggi_area(computer);
            distruggi_area(umano);
            return 42;//ritorno 42 se vince computer
        }
        e=ERRORE;
    }
    return EXIT_FAILURE;//ritorno 1 se si è verificato un errore.
}


float gioca2(unsigned n, unsigned m, unsigned n_navi1, unsigned n_navi2, unsigned n_navi3, int diff){//funzione che gestisce un'intera partita
    area_t*umano;
    area_t*computer;
    float e_pc=0;
    int e=ERRORE;
    umano=crea_area(n,m);//alloco e inizializzo le aree.
    computer=crea_area(n,m);
    if (umano==NULL) return EXIT_FAILURE;
    if (computer==NULL) return EXIT_FAILURE;
    while (put(1,n_navi1, n_navi2, n_navi3, umano, computer)==-1) init(umano); //inserisco le navi. Finché l'insermento non va a buon fine ripeto inizializzando l'area di gioco.
    while(1){//blocco partita
        while(e==ERRORE)//turno UMANO
        {
            e = attacco_computer(diff, computer);
        }//impossibile colpire caselle già colpite. Nel caso la funzione esegui_mossa ritorna Errore e il blocco while si ripete.
        //stampa_esito (e);
        umano->shots++;//aggiorno il numero di tiri
        if(vittoria(computer)==0){
            e_pc=(float)computer->hit/umano->shots;//verifico che il numero di navi del computer sia uguale a 0.
            distruggi_area(computer);//dealloco le aree
            distruggi_area(umano);
            return e_pc;//ritorno il rapporto fra colpite e tiri
        }
        e=ERRORE;//TURNO COMPUTER
        while(e==ERRORE) e=attacco_computer(diff, umano);//chiamo la funzione per l'attacco del computer e attacco finché non colpisco una casella libera
        computer->shots++;//aggiorno tiri
        if(vittoria(umano)==0){//verifico che le navi dell'umano sia 0
            e_pc=(float)computer->hit/umano->shots;
            distruggi_area(computer);
            distruggi_area(umano);
            return e_pc;//ritorno il rapporto fra colpite e tiri
        }
        e=ERRORE;
    }
    return EXIT_FAILURE;//ritorno 1 se si è verificato un errore
}

