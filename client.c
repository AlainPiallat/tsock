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
    char init_msg[9];
    snprintf(init_msg, sizeof(init_msg), "E%d%d%d", length, count, number);
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
    char init_msg[5];
    snprintf(init_msg, sizeof(init_msg), "R%d", number);
    if (send(sock, init_msg, sizeof(init_msg), 0) < 0) {
        perror("Erreur lors de l'envoi du message d'initialisation");
        close(sock);
        return -1;
    } else {
        printf("Récepteur : demande de messages pour la boîte aux lettres %d\n", number);
    }

    // Réception des messages
    char buffer[1024];
    int bytes_received;
    while ((bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the received data
        printf("Message reçu : %s\n", buffer);
    }
    if (bytes_received < 0) {
        perror("Erreur lors de la réception des messages");
        close(sock);
        return -1;
    }

    // Fermeture de la connexion
    close(sock);
    return 0;
}