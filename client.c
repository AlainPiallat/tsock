#include "client.h"
#include <arpa/inet.h>

/**
 * Cette fonction permet à un émetteur de se connecter au serveur pour envoyer des messages.
 * L'émetteur établit une connexion TCP avec le serveur, envoie les messages, puis ferme la connexion.
 *
 * @param hostname Nom d'hôte du serveur.
 * @param port Port du serveur.
 * @param length Longueur des messages à envoyer.
 * @param count Nombre de messages à envoyer.
 * @param number Numéro de la boîte aux lettres destinataire.
 * @return 0 si l'émetteur a réussi, -1 sinon.
 */
int emitter(char *hostname, int port, int length, int count, int number) {
    int sock;
    struct sockaddr_in server_addr;
    char *message;

    // Création du socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la création du socket");
        return -1;
    }

    // Configuration de l'adresse du serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, hostname, &server_addr.sin_addr) <= 0) {
        perror("Adresse invalide ou non supportée");
        close(sock);
        return -1;
    }

    // Connexion au serveur
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur lors de la connexion au serveur");
        close(sock);
        return -1;
    } else {
        printf("Connecté au serveur %s sur le port %d\n", hostname, port);
    }
    
    // Envoi du message d'initialisation
    char init_msg[14];
    union int_char length_str;
    union int_char count_str;
    union int_char number_str;
    length_str.i = length;
    count_str.i = count;
    number_str.i = number;
    strcpy(init_msg, "E");
    strncpy(init_msg + 1, length_str.c, 4);
    strncpy(init_msg + 5, count_str.c, 4);
    strncpy(init_msg + 9, number_str.c, 4);
    init_msg[13] = '\0'; // Null-terminate the string
    if (send(sock, init_msg, sizeof(init_msg), 0) < 0) {
        perror("Erreur lors de l'envoi du message d'initialisation");
        close(sock);
        return -1;
    } else {
        printf("Emetteur : envoi de %d messages de longueur %d à la boîte aux lettres %d\n", count, length, number);
    }

    // Envoi des messages
    message = malloc(length);
    if (!message) {
        perror("Erreur d'allocation mémoire");
        close(sock);
        return -1;
    }
    memset(message, 'A', length); // Génération d'un message de test
    for (int i = 0; i < count; i++) {
        if (send(sock, message, length, 0) < 0) {
            perror("Erreur lors de l'envoi d'un message");
            free(message);
            close(sock);
            return -1;
        } else {
            printf("EMETTEUR: Envoi n°%d (%d) [%s]\n", i + 1, length, message);
        }
    }
    free(message);

    // Fermeture de la connexion
    close(sock);
    return 0;
}

/**
 * Cette fonction permet à un récepteur de se connecter au serveur pour récupérer des messages.
 * Le récepteur établit une connexion TCP avec le serveur, envoie le numéro de sa boîte aux lettres,
 * récupère les messages, puis ferme la connexion.
 *
 * @param hostname Nom d'hôte du serveur.
 * @param port Port du serveur.
 * @param number Numéro de la boîte aux lettres.
 * @return 0 si le récepteur a réussi, -1 sinon.
 */
int receiver(char *hostname, int port, int number) {
    int sock;
    struct sockaddr_in server_addr;

    // Création du socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la création du socket");
        return -1;
    }

    // Configuration de l'adresse du serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, hostname, &server_addr.sin_addr) <= 0) {
        perror("Adresse invalide ou non supportée");
        close(sock);
        return -1;
    }

    // Connexion au serveur
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur lors de la connexion au serveur");
        close(sock);
        return -1;
    } else {
        printf("Connecté au serveur %s sur le port %d\n", hostname, port);
    }

    // Envoi du message d'initialisation
    char init_msg[6];
    union int_char number_str;
    number_str.i = number;
    strcpy(init_msg, "R");
    strncpy(init_msg + 1, number_str.c, 4);
    init_msg[5] = '\0'; // Null-terminate the string
    if (send(sock, init_msg, sizeof(init_msg), 0) < 0) {
        perror("Erreur lors de l'envoi du message d'initialisation");
        close(sock);
        return -1;
    } else {
        printf("Récepteur : demande de messages pour la boîte aux lettres %d\n", number);
    }

    // Réception des messages
    // Pour chaque message, on reçoit d'abord la longueur du message
    int i = 0;
    union int_char length_str;
    while (recv(sock, length_str.c, 4, 0) > 0) {
        int length = length_str.i;
        char *message = malloc(length);
        if (!message) {
            perror("Erreur d'allocation mémoire");
            close(sock);
            return -1;
        }
        if (recv(sock, message, length, 0) < 0) {
            perror("Erreur lors de la réception d'un message");
            free(message);
            close(sock);
            return -1;
        } else {
            printf("RECEPTEUR: Réception n°%d (%d) [%s]\n", ++i, length, message);
        }
        free(message);
    }
    

    // Fermeture de la connexion
    close(sock);
    return 0;
}