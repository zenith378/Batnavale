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

test1:
	rm -f ./out1.check
	cp DATA/out1.check .
	make cleanall
	make bat1
	./bat1 | tee out1.txt
	diff -s out1.txt out1.check # (controlla che l'output sia uguale a DATA/out1.check)
	@echo "********** Test1 superato!"

test2: 
	make cleanall
	make bat2
	./bat2 
	@echo "********** Test2 superato!"

test3: 
	rm -f ./out3.check
	cp DATA/out3.check .
	cp DATA/mosse.txt .
	make cleanall
	make bat3 
	./bat3 < mosse.txt > out3.txt
	diff -s out3.txt out3.check # (controlla che l'output sia uguale a DATA/out3.check)
	@echo "********** Test3 superato!"

	

test3debug: 
	make cleanall
	make bat3 
	./bat3 
	@echo "********** Test3 debug superato!"

test4: 
	make cleanall
	make bat4
	./bat4
	@echo "********** Test3 superato!"

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


consegna:
	make cleanall
	make test1
	make test2
	make test3
	./gruppo-check.pl < gruppo.txt
	tar -cvf $(USER)-a2.tar ./gruppo.txt ./05test.c ./06test.c ./gioca.c ./bnavale_muta.h ./bnavale.h ./regole.txt ./battleship.pdf ./README1.txt ./Makefile $(FILE_DA_CONSEGNARE) 
	@echo "********** TAR creato inviare in allegato a una mail al docente del proprio corso insieme a nome, cognome,  numero di matricola dei componenti del gruppo"
