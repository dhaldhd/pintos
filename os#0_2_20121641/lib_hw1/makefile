all : main

main : main.o list.o hash.o bitmap.o
	gcc -o testlib main.o list.o hash.o bitmap.o

main.o : main.c
	gcc -c main.c -Wall

list.o: list.c
	gcc -c list.c -Wall

hash.o: hash.c
	gcc -c hash.c -Wall

bitmap.o: bitmap.c bitmap.h
	gcc -c bitmap.c -Wall

clean : 
	rm -f *.o 20121641

