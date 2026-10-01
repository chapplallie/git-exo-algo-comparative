# Exercice 7 : on reprend le code de l'exercice 6 (compteur + fuite avec -DFUITE)
# et on le passe dans valgrind, ou dans le desinfecteur d'adresses si pas de valgrind.
# A lancer depuis exercice7/ (sous Linux / WSL pour valgrind) :
#   make valgrind         -> version sans fuite
#   make valgrind-fuite   -> version avec la 2e liste pas liberee
#   make asan / make asan-fuite -> pareil avec -fsanitize=address

CC     = gcc
# -g obligatoire sinon valgrind ne donne pas les numeros de ligne
CFLAGS = -Wall -Wextra -std=c11 -g
DIR    = ../exercice6
SRC    = $(DIR)/main.c $(DIR)/liste.c

demo: $(SRC) $(DIR)/liste.h
	$(CC) $(CFLAGS) -o demo $(SRC)

demo-fuite: $(SRC) $(DIR)/liste.h
	$(CC) $(CFLAGS) -DFUITE -o demo-fuite $(SRC)

valgrind: demo
	valgrind --leak-check=full ./demo

valgrind-fuite: demo-fuite
	valgrind --leak-check=full ./demo-fuite

asan: $(SRC) $(DIR)/liste.h
	$(CC) $(CFLAGS) -fsanitize=address -o demo-asan $(SRC)
	./demo-asan

asan-fuite: $(SRC) $(DIR)/liste.h
	$(CC) $(CFLAGS) -DFUITE -fsanitize=address -o demo-asan $(SRC)
	./demo-asan

clean:
	rm -f demo demo.exe demo-fuite demo-fuite.exe demo-asan demo-asan.exe

.PHONY: valgrind valgrind-fuite asan asan-fuite clean
