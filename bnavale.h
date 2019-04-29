/*
  \file
  \authors informatica (CDS Fisica) 2018/19
  \brief A2: file di include
 */

/** tipo di ogni cella in sui e' suddiviso il mare (area di gioco)
  -- EMPTY, cella vuota, 
  -- ONE contenente una nave lunga uno, 
  -- TWO, THREE parte di una nave lunga 2 o parte di una nave lunga 3 */

#define EMPTY  '.'
#define ONE    '1'
#define TWO    '2'
#define THREE  '3'
#define EMPTY_COLPITA  'X'
#define ONE_COLPITA    'a'
#define TWO_COLPITA    'b'
#define THREE_COLPITA  'c'



typedef struct area {
  /* matrice che rappresenta l'area di gioco */
  char ** a;
  /* numero di righe */
  unsigned N;
  /* numero di colonne */
  unsigned M;
  /* numero di navi presenti */
  unsigned n_navi;
    /*numero di caselle mancate SUBITE*/
    unsigned misses;
    /*numero di caselle colpite SUBITE*/
    unsigned hit;
    /*numero di tiri totali effettuati*/
    unsigned shots;
} area_t;

/* Funzione utilizzata per svuotare la stdin quando si usa la scanf per leggere un carattere inserito dall'utente in momenti precisi del gioco */

void empty_stdin (void);
/* funzione BUFFER: ha lo scopo di controllare il corretto funzionamento della funzione scanf in tutti i casi in cui viene letto ed assegnato un valore inserito dall'utente in stdin durante il gioco 
    \param rtn valore di ritorno scanf
    \retval 0 se è andato tutto bene
    \retval -1 se si è verificato un errore*/


int buffer (int rtn);
/** alloca l'area di gioco  nXm suddivisa in celle di uguali dimensioni vuote
    con condizioni periodiche di bordo 
    (n-1 adiacente a 0 in tutte le dimensioni) 
    \param n numero delle righe
    \param m numero delle colonne

    \retval p puntatore alla nuova area (Se tutto e' andato bene)
    \retval NULL se si e' verificato un errore
*/
area_t * crea_area (unsigned n, unsigned m) ;

/** dealloca l'area di gioco in tutte le sue parti
    \param sea area da deallocare
*/
void distruggi_area (area_t * sea) ;

/**
   inizializza l'area di gioco tutta a EMPTY 
   \param sea l'area di gioco da inizializzare
*/
void init (area_t * sea);


/**
   inserisce nell'area di gioco una nuova nave lunga 1 (posizionamento casuale)
   \param sea l'area di gioco

   \retval 0 se l'inserimento è andato a buon fine
   \retval -1 se si è verificato un errore (es. non c'e' posto)
*/
int put_one (area_t* sea);

/**
   inserisce nell'area di gioco una nuova nave lunga 2 (posizionamento casuale e orientamento casuale della nave)
   \param sea area di gioco

   \retval 0 se l'inserimento è andato a buon fine
   \retval -1 se si è verificato un errore (es. non c'e' posto)
*/
int put_two (area_t * sea);

/**
   inserisce nell'area di gioco una nuova nave lunga 3 (posizionamento casuale e orientamento casuale della nave)
   \param sea area di gioco

   \retval 0 se l'inserimento è andato a buon fine
   \retval -1 se si è verificato un errore (es. non c'e' posto)
*/
int put_three (area_t* sea);

/**
   stampa fomattata del mare su stdout (FORNITO DAI DOCENTI NON MODIFICARE -- file bnavale_docenti.c)
   \param sea area da stampare
*/
void print_area (area_t* sea);

/**
  conta le navi di tipo k in sea, incluse navi colpite o affondate (FORNITO DAI DOCENTI NON MODIFICARE -- file bnavale_docenti.c)
  \param k tipo di navi da contare ONE, TWO o THREE

  \retval n numero delle navi di tipo k, o -1 se k non è ONE, TWO o THREE
*/
int conta_navi (area_t* sea, char k);

/** esiti possibili della funzione esegui_mossa */
#define ERRORE          130
#define ACQUA            131
#define AFFONDATA_ONE    132
#define AFFONDATA_TWO   133
#define AFFONDATA_THREE 134
#define COLPITA_TWO  135
#define COLPITA_THREE  136

/**
   Controlla l'esito del colpo di coordinate (x,y) nell'area di gioco
   \param sea area di gioco
   \param x,y coordinate del colpo

   \retval e uno dei valori di esito definiti sopra 

 */
int esegui_mossa (area_t * sea, int x, int y);

/**
   stampa un messaggio di errore in base all'esito (fornita dai docenti nel file bnavale_docenti.c NON MODIFICARE  -- file bnavale_docenti.c)
   \param e uno dei valori di esito definiti sopra */
   void stampa_esito (int e);

/** funzione crea le due aree di gioco e gestisce una partita con il computer. Si puo' assumere che l'umano giochi per primo. 
    \param nXm ampiezza area di gioco
    \param n_navi1 numero navi da 1 inserite casulamente nelle due aree
    \param n_navi2 numero navi da 2 inserite casulamente nelle due aree
    \param n_navi3 numero navi da 3 inserite casulamente nelle due aree */
void gioca(unsigned n, unsigned m, unsigned n_navi1, unsigned n_navi2, unsigned n_navi3);


//funzione della stampa dell'area di gioco modificata per permettere di visualizzare il numero di righe e colonne.
/*Sopra la prima riga viene visualizzato un numero intero (0,1,2...), sopra la prima colonna la lettera (A,B,C...).*/

void print_area_mia (area_t * sea);

/*Funzione che stampa uno schema ausiliario utile nella partita. Sono presenti due griglie. La griglia di sinistra è uguale a quella stamapta in print_area_mia e rappresenta il proprio mare con le proprie navi. La griglia di destra mostra la situazione conosciuta sulla matrice dell'avversario, ovvero filtra la matrice dell'avversario mostrando solo i caratteri X, a, b e c. I parametri sono le due struc (umano e computer)t. I comandi per adattare la visualizzazione della griglia (numero colonna, spazi, confini della matrice...) sono gli stessi della funzione print_area_mia */

void schema(area_t*sea,area_t*computer);


/**Funzione che verifica che sia effettivamente possibile inserire una nave di dimensione dim nella direzione dir in posizione i+1 j+1 sulla matrice della struct sea. Viene chiamata dopo che il giocatore o il computer hanno inserito/generato delle coordinate di inserimento di una nave, con annessa dimensione della nave e direzione di inserimento. Restituisce il valore 0 se l'inserimento nella data condizione è possibile oppure -1 se è impossibile. L'errore verrà poi gestito da altre funzioni che inviteranno a inserire delle nuove coordinate
    \param dim dimensione name
    \param i,j coordinate riga, colonna da verificare
    \param sea puntatore a struct
    \param dir direzione
    \retval 0 se è possibile inserire una nave nella casella di coordinate i+1,j+1
    \retval -1 se non è possibile*/

int verify_spec(int dim,int i,int j,area_t*sea,int dir);

/**funzione che verifica la presenza di uno slot 3x(dim+2) o (dim+2)x3 libero per inserire una nave. E' un controllo globale sulla matrice richiamando casella per casella la funzione verify spec e serve ad individuare la situazione per cui è impossibile completare l'inserimento delle navi richieste a causa della particolare disposizione generata/inserita, scongiurando il caso di loop in cui vengono continuamente richieste/generate nuove coordinate in una griglia in cui è impossibile inserire la nave richiesta.
 \retval 0 se è possibile inserire una nave
 \retval -1 se non è possibile
 */
int verify(area_t*sea,int dim);

/*funzione che inserisce una nave di dimensione dim nella griglia della struct sea. L'inserimento può essere automatico o manuale a seconda del parametro man, che viene preso in input nel corso del gioco sulla base della scelta dell'utente: man==1 inserimento manuale. man==0 automatico*/

int put_man(area_t*sea,int dim,int man);

/*Funzione che gestisce l'interfaccia con l'utente per l'inserimento delle navi. La funzione inoltre va ad inserire nelle matrici di entrambe le struct il numero esatto di navi passato alla funzione. Prende in input le variabili scelta(inserimento manuale o automatico), n_navi ripettivamente per le navi da 1,2,3, le griglie del giocatore e del computer*/

int put(int scelta, unsigned n_navi1, unsigned n_navi2, unsigned n_navi3,area_t*umano,area_t*computer);

//gestisce l'interfaccia con l'utente perla scelta di inserimento delle navi(modo automatico o manuale) e chiama le funzioni di inserimento

int inser_navi(unsigned n_navi1, unsigned n_navi2, unsigned n_navi3, area_t*umano,area_t*computer);

/*Funzione che regola le impostazioni di gioco (default/custom) e in caso i parametri (dimensioni griglia, num navi) all'apertura del gioco. Inoltre la funziona controlla, in caso di scelta manuale, che in una griglia di dimensione nxm possa effettivamente essere inserito il numero di navi desiderato dall'utente.*/
//i parametri della funzione, passati come puntatori saranno usati come valori di ritorno per inserirli nella funzione gioca.
void custom(int*num_navi1,int*num_navi2,int*num_navi3,int*N,int*M);

//funzione che verifica se la nave da tre è affondata o solo colpita. In caso la nave sia affondata la funzione ritorna 0, altrimenti -1. E' distinta dai casi da 1 e 2 per la sua struttura leggermente più complessa

int affondata_tre(area_t*sea,int x,int y);

/*funzione di controllo per determinare la vittoria. non appena il numero di navi è zero ritorna 0.*/
int vittoria(area_t* sea);

/*visualizzazione delle statistiche. Riporto i valori della struct, che sono aggiornati ad ogni turno sulla base delle mosse.*/
void stats(area_t*sea, area_t*sea2);

/*Schermata di benvenuto. Il disegno è di sagnew https://github.com/sagnew/Battleship
 La funzione permette di leggere le regole contenute nel file "regole.txt"
 */
void superwelcomescreen();

/**Funzione che implementa le condizioni periodiche di bordo.
 \param i elemento della matrice
 \param n colonne d'ercole.
 \retval nuova posizione dell'elemento.
 */
int cond_board(int i, int n);


/*Funzione ausiliaria per la difficoltà intermedia (all'avvio del gioco verrà chiesto all'utente di selezionare la difficoltà desiderata). Il computer controlla che in caso abbia colpito una casella EMPTY nelle caselle adiacenti non ci siano pezzi di navi già colpite. In caso le navi colpite siano effettivamente presenti la funzione ritorna -1. Poichè nei dintorni di una nave non possono essere inserite altre navi la funzione scarta il tiro e ne genera un altro , di modo da colpire solo caselle in cui la disposizione di navi è permessa.
\param x e y coordinate tiro
\retval -1 in caso sono presenti navi colpite nelle caselle adiacenti*/
int intermedia(area_t*sea, int x, int y);

//funzione ausiliaria per la modalità difficile. Verifica che, data una casella TWO_COLPITA o THREE_Colpito, nelle caselle adiacenti ci siano pezzi di nave già colpiti. In caso ci siano o la funzione non abbia in input TWO_Colpita o Three_colpita la funzione ritorna -1. In caso non ci siano e la casella sia effettivamente two_colpita e three_colpita la funzione ritorna 0. L'idea è quella di implementare un attacco che, una volta colpita una parte di nave, colpisca le caselle adiacenti fino all'affondamento nei turni successivi. Questo viene implementato successivamente nella funzione attacco_computer sulla base del valore restituito dalla funzione difficile //
int difficile(area_t*sea,int x,int y,int dim);

/*funzione che gestisce l'attacco del computer a seconda del livello selezionato.
 \retval e=esito (acqua,colpita_one etc. a seconda delle macro)*/
int attacco_computer(int livello, area_t*sea);

//funzione per scegliere il livello di difficoltà
int diffi();

/*Funzione per decidere chi gioca per primo*/
int primo();
