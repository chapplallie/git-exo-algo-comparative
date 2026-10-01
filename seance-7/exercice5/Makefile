# Exercice 5 : version FAUTIVE expres, liste.h retire des dependances de main.o.
# A lancer depuis exercice5/ : mingw32-make, puis touch ../liste.h, puis mingw32-make
# (le bon Makefile est celui de seance-7/)

demo: main.o liste.o
	gcc -Wall -Wextra -std=c11 -g -o demo main.o liste.o

main.o: ../main.c
	gcc -Wall -Wextra -std=c11 -g -c ../main.c

liste.o: ../liste.c ../liste.h
	gcc -Wall -Wextra -std=c11 -g -c ../liste.c

clean:
	rm -f main.o liste.o demo demo.exe

.PHONY: clean
