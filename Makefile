################################
#
#     Makefile SECONDO ASSEGNAMENTO AA 2018/19
#    
################################


# file da consegnare
# modificare se si aggiungono file
FILE_DA_CONSEGNARE=bnavale.c bnavale_muta.c

# preferenze compilatore e flag di compilazione
CC=gcc
# compilazione 
CFLAGS= -Wall -pedantic -g

obj = bnavale.o bnavale_docenti.o
obj1 = bnavale_muta.o bnavale_docenti.o



# phony targets
.PHONY: cleanall test1 test2 test3 consegna 


bat1 : 01test.o $(obj)
	gcc $(obj) 01test.o -o bat1 -lm

bat2 : 02test.o $(obj)
	gcc $(obj) 02test.o -o bat2 -lm

bat3 : 03test.o $(obj)
	gcc $(obj) 03test.o -o bat3 -lm

bat4 : 04test.o $(obj)
	gcc $(obj) 04test.o -o bat4 -lm

bat5 : 05test.o $(obj1)
	gcc $(obj1) 05test.o -o bat5 -lm

bat6 : 06test.o $(obj1)
	gcc $(obj1) 06test.o -o bat6 -lm
bat7 : gioca.o $(obj)
	gcc $(obj) gioca.o -o bat7 -lm


bnavale.o: bnavale.c bnavale.h  
bnavale_muta.o: bnavale_muta.o bnavale_muta.h
bmain.o: bmain.c bnavale.h bnavale_muta.h 
01test.o: 01test.c bnavale.h   
02test.o: 02test.c bnavale.h   
03test.o: 03test.c bnavale.h   
04test.o: 04test.c bnavale.h   
05test.o: 05test.c bnavale_muta.h
06test.o: 06test.c bnavale_muta.h
gioca.o: gioca.c bnavale.h

battaglia : $(obj) bmain.o
	gcc $(obj) bmain.o -o battaglia -lm

battaglia1 : $(obj1) bmain.o
	gcc $(obj1) bmain.o -o battaglia1 -lm



cleanall: 
	-rm -f *.o *~ ./core bat? battaglia

cleanall1:
	-rm -f *.o *~ ./core bat? battaglia1


test5:
	make cleanall1
	make bat5
	./bat5
	@echo "********** Test5 superato!"

test6: 
	make cleanall
	make bat6
	./bat6
	@echo "********** Test6 superato!"

gioca:
	make cleanall
	make bat7
	./bat7
