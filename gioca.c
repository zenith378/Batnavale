//
//  06test.c
//  
//
//  Created by Giulio Cordova on 24/04/19.
//
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bnavale.h"

int main (void) {
    int n=8,m=8,n_navi1=1,n_navi2=2,n_navi3=3;
    superwelcomescreen();
    srand(time(NULL));
    custom(&n_navi1,&n_navi2,&n_navi3,&n,&m);
    gioca(n,m,n_navi1,n_navi2,n_navi3);
    return 0;
}
