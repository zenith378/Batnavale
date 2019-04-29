//
//  05test.c
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
#define navi3 5

int main (void) {
    int i,diff,cicli;
    double uomo=0,pc=0,male=0,flag=0;//variabile uomo: partite vinte dall'uomo. variabile pc: partite vinte dal pc. variabile male: partite che non sono state terminate né con la vittoria dell'uomo, né con la vittoria del computer.
    
    fprintf(stdout,"Test 5: Inserisci il numero di cicli da verificare\n");
    int rtn=scanf("%d",&cicli);//inserimento #cicli
    buffer(rtn);//gestione scanf
    diff=diffi();//inserimento difficoltà
    
    /* inizializzo il generatore dei numeri casuali */
    srand(time(NULL));
    
    /* avvio il test */
    for(i=0;i<cicli;i++){
        flag++;//flag per verificare se il programa è in loop.
        switch(gioca1(10,10,2,3,4,diff)){
            case 0: uomo++;//vince l'uomo, aggiorno uomo.
                break;
            case 42: pc++; //vince PC, aggiorno PC
                break;
            default: male++;//la funzione non ha ritornato né 0 né 42, per cui qualcosa è andato storto: aggiorno male
                break;
        }
    }
    printf("uomo:%.2lf%%, computer:%.2lf%%, male:%.2lf%%\n",uomo*100/cicli,pc*100/cicli,male*100/cicli);//stampo la percentuale di vittorie
    return 0;
    
    
}

