#include "Utils.h"

int string_to_int(const char *str) {
    int result = 0;
    int i = 0;
    
    // Gérer les espaces blancs au début
    while (str[i] == ' ') {
        i++;
    }

    // Gérer le signe négatif
    int sign = 1;
    if (str[i] == '-') {
        sign = -1;
        i++;
    }

    // Convertir chaque caractère en entier
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return sign * result;
}

// Fonction pour générer le message tsock
char* genere_message(int n, int N) {
    // Allocation pour le message, taille totale (N caractères + 1 pour le '\0')
    char *message = (char*) malloc(N + 1); 
    if (message == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(1);
    }

    // Génération de la partie numérique avec un format de 5 caractères
    snprintf(message, 6, "%5d", n);  // %5d : remplissage avec des espaces à gauche jusqu'à 5 caractères

    // Génération de la partie lettre
	// Calcul de la lettre en fonction de n % 26
	char letter = 'a' + ((n-1) % 26) ;
    for (int i = 0; i < N-5; i++) {
        message[5 + i] = letter;  // 5 car les premier caractère du message est réservé au nombre + espace
    }

    // Ajouter un '\0' à la fin du message pour le rendre une chaîne valide
    message[N+1] = '\0';

    return message;
}

int construire_adresse_local(int port, struct sockaddr_in* addr){
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = INADDR_ANY;
	return 0;
}

int construire_adresse_dest(struct hostent * dest, int port, struct sockaddr_in* addr){
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	memcpy((char*)&(addr->sin_addr.s_addr),
		dest->h_addr_list[0],
		dest->h_length);
	return 0;
}
