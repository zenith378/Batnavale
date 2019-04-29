//
//  test7.c
//  Batnavale
//
//  Created by Giulio Cordova on 17/04/19.
//  Copyright © 2019 Giulio Cordova. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "bnavale_muta.h"

#define N 10
#define M 10
#define navi1 2
#define navi2 3
#define navi3 4
#define SEED 42

int main (void) {
    int i,diff,cicli;
    double flag=0,e=0;//flag: variabile usata nel debugging per verificare che il programma non ciclasse, e: variabile che rappresenta l'efficacia dell'attacco.
    
    fprintf(stdout,"Test 5: Inserisci il numero di cicli da verificare\n");
    int rtn=scanf("%d",&cicli);
    buffer(rtn);
    diff=diffi();
    
    /* inizializzo il generatore dei numeri casuali */
    srand(SEED);
    
    /* avvio il gioco interattivo */
    for(i=0;i<cicli;i++){
        flag++;//aggiorno la flag
        e+=gioca2(N,M,navi1,navi2,navi3,diff);//aggiorno il valore di e.
    }
    printf("efficenza attacco=%.2lf%%\n",e*100/cicli);
    return 0;
    
    
}

