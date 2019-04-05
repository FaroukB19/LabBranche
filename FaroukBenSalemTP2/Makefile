#declare the variable
CC=g++

CFLAGS=-C


all: tp2

tp2: main.o
	g++ main.cpp -o tp2.o

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp


clean:
	rm -rf *o tp2

