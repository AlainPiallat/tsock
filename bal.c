#include "bal.h"


int server_sender(int sock, mailbox *mbox, int length, int count) {
    for (int i = 0; i < count; i++) {
        // Allouer un buffer pour recevoir le message
        char *message = (char *)malloc(length);
        if (message == NULL) {
            perror("SERVER_SENDER: Memory allocation failed");
            close(sock);
            return -1;
        }

        // Recevoir le message depuis le socket
        int received = recv(sock, message, length, 0);
        if (received <= 0) {
            perror("SERVER_SENDER: Error receiving message");
            free(message);
            close(sock);
            return -1;
        }

        // Ajouter le message à la boîte aux lettres
        if (add_message(mbox, message, length) != 0) {
            printf("SERVER_SENDER: Failed to add message to mailbox\n");
            free(message);
            close(sock);
            return -1;
        }

        // Libérer le buffer après ajout
        free(message);
    }

    // Fermer le socket après réception de tous les messages
    shutdown(sock, 2);
    close(sock);
    return 0;
}

// Fonction pour gérer la réception des messages par le récepteur
int server_receiver(int sock, mailbox *mbox) {
    while (1) {
        // Récupérer un message depuis la boîte aux lettres
        int length;
        char *message = get_message(mbox, &length);
        if (message == NULL) {
            // Plus de messages à envoyer
            break;
        }

        // Envoyer le message au récepteur via le socket
        int sent = send(sock, message, length, 0);
        if (sent <= 0) {
            perror("SERVER_RECEIVER: Error sending message");
            free(message);
            close(sock);
            return -1;
        }

        // Libérer le buffer après envoi
        free(message);
    }

    // Fermer le socket après envoi de tous les messages
    shutdown(sock, 2);
    close(sock);
    return 0;
}

int server(int port) {
    // Créer un socket d'écoute
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // Lier le socket à l'adresse du serveur
    struct sockaddr_in* local_address = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));

    construire_adresse_local(port, local_address);

    // Écouter les connexions entrantes
    listen(sock, 5);

    // Créer la liste des boîtes aux lettres
    mailbox* mbox_list = NULL;
    initialize_mailbox(&mbox_list);

    // Afficher les informations du serveur
    printf("SERVER: listening on port %d\n", port);
    printf("SERVER: waiting for connections...\n");

    // Accepter les connexions entrantes
    while (1) {
        struct sockaddr_in* source_address = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
        int source_size = sizeof(struct sockaddr_in);
        // Accepter une nouvelle connexion
        int allocated_sock = accept(sock, source_address, &source_size);
        if (allocated_sock != -1) {
            continue;
        }
        
        // Récupérer le message d'initialisation
        char message[30];
        int message_size = sizeof(message);
        recv(allocated_sock, message, message_size, 0);
        printf("SERVER: initialisation message: %s\n", message);
        if (message[0] == 'E') {
            // Récupérer la taille des messages
            char* message_length_str[8];
            strcpy(message_length_str, message + 1);
            message_length_str[8] = '\0';
            int message_length = string_to_int(message_length_str);

            // Récupérer le nombre de messages
            char* message_count_str[8];
            strcpy(message_count_str, message + 9);
            message_count_str[8] = '\0';
            int message_count = string_to_int(message_count_str);

            // Récupérer le numéro de la boîte aux lettres
            char* mailbox_number_str[8];
            strcpy(mailbox_number_str, message + 17);
            mailbox_number_str[8] = '\0';
            int mailbox_number = string_to_int(mailbox_number_str);

            // Afficher les informations récupérées
            printf("SERVER: message_length=%d, message_count=%d, mailbox_number=%d\n", message_length, message_count, mailbox_number);

            // récupérer la boîte aux lettres
            mailbox* mbox = get_mailbox(&mbox_list, mailbox_number);
            if (mbox == NULL) {
                printf("SERVER: mailbox not created\n");
                continue;
            }

            // TODO Appeler la fonction d'envoi
            if (server_sender(allocated_sock, mbox, message_length, message_count) != 0) {
                printf("SERVER: Error in server_sender\n");
                continue;
            }
        } else if (message[0] == 'R') {
            // Récupérer le numéro de la boîte aux lettres
            char* mailbox_number_str[8];
            strcpy(mailbox_number_str, message + 1);
            mailbox_number_str[8] = '\0';
            int mailbox_number = string_to_int(mailbox_number_str);

            // Afficher les informations récupérées
            printf("SERVER: mailbox_number=%d\n", mailbox_number);

            // récupérer la boîte aux lettres
            mailbox* mbox = get_mailbox(&mbox_list, mailbox_number);
            if (mbox == NULL) {
                printf("SERVER: mailbox not created\n");
                continue;
            }
            
            // TODO Appeler la fonction de réception
            if (server_receiver(allocated_sock, mbox) != 0) {
                printf("SERVER: Error in server_receiver\n");
                continue;
            }
        }
    }

    // Libérer la mémoire allouée pour les boîtes aux lettres
    free_mailboxes(&mbox_list);

    // Fermer le socket après utilisation
    close(sock);
    return 0;
}