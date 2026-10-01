# Exercice 6 : meme Makefile que celui de seance-7/, plus une cible "fuite"
# qui compile avec -DFUITE pour activer la 2e liste jamais liberee.
# A lancer depuis exercice6/ : make    puis    make fuite

CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
OBJ    = main.o liste.o

demo: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c liste.h
	$(CC) $(CFLAGS) -c $< -o $@

fuite: main.c liste.c liste.h
	$(CC) $(CFLAGS) -DFUITE -o demo-fuite main.c liste.c

clean:
	rm -f $(OBJ) demo demo.exe demo-fuite demo-fuite.exe

.PHONY: clean fuite
