# Exercice 4 : premier Makefile, ecrit a la main.
# Les sources sont dans le dossier parent (seance-7/), d'ou les ../
# A lancer depuis exercice4/ : mingw32-make (ou make)

demo: main.o liste.o
	gcc -Wall -Wextra -std=c11 -g -o demo main.o liste.o

main.o: ../main.c ../liste.h
	gcc -Wall -Wextra -std=c11 -g -c ../main.c

liste.o: ../liste.c ../liste.h
	gcc -Wall -Wextra -std=c11 -g -c ../liste.c

clean:
	rm -f main.o liste.o demo demo.exe

.PHONY: clean
