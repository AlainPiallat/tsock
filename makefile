CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -pthread

# Liste des fichiers objets
OBJS = main.o client.o bal.o chained_list.o Utils.o

# Cible principale
all: main

# Génération de l'exécutable
main: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# Règle générique pour compiler les fichiers .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f main
	rm -f *.o
