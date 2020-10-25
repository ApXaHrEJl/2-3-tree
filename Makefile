all: bin/ex

bin/ex: obj/main.o obj/tree23.o
	mkdir bin
	gcc -g -Wall obj/main.o obj/tree23.o -o bin/ex

obj/tree23.o: src/tree23.c 
	gcc -g -I include -Wall -c src/tree23.c -o obj/tree23.o

obj/main.o: src/main.c
	mkdir obj
	gcc -g -I include -Wall -c src/main.c -o obj/main.o
clean:
	rm -rf bin/ obj/

PHONY: all clean
