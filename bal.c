#include "bal.h"


int server_sender(int sock, mailbox *mbox, int length, int count) {
    // Informer de la création du socket
    printf("SERVER_SENDER: Socket created\n");
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

        // Informer de l'ajout du message
        printf("SERVER_SENDER: Message added to mailbox %d\n", mbox->number);

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
    // Informer de la création du socket
    printf("SERVER_RECEIVER: Socket created\n");
    int length;
    char *message;
    while ((message = get_message(mbox, &length)) != NULL) {
        // Envoyer la taille du message au récepteur
        union int_char message_length_str;
        message_length_str.i = length;
        if (send(sock, message_length_str.c, sizeof(int), 0) <= 0) {
            perror("SERVER_RECEIVER: Error sending message length");
            free(message);
            close(sock);
            return -1;
        }

        // Envoyer le message au récepteur via le socket
        if (send(sock, message, length, 0) <= 0) {
            perror("SERVER_RECEIVER: Error sending message");
            free(message);
            close(sock);
            return -1;
        }

        // Informer de l'envoi du message
        printf("SERVER_RECEIVER: Message sent to receiver\n");

        // Libérer le buffer après envoi
        free(message);
    }

    // Informer de la fin de l'envoi
    printf("SERVER_RECEIVER: No more messages to send\n");

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
    
    // Lier le socket à l'adresse locale 
    bind(sock,(struct sockaddr *) local_address, sizeof(struct sockaddr_in));

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
        socklen_t source_size = (socklen_t) sizeof(struct sockaddr_in);
        // Accepter une nouvelle connexion
        int allocated_sock = accept(sock, (struct sockaddr *)source_address, &source_size);
        if (allocated_sock == -1) {
            perror("SERVER: socket not created");
            continue;
        }
        
        // Récupérer le message d'initialisation
        char message[14];
        int message_size = sizeof(message);
        recv(allocated_sock, message, message_size, 0);
        printf("SERVER: initialisation message received\n");
        if (message[0] == 'E') {
            // Récupérer la taille des messages
            union int_char message_length_str;
            strncpy(message_length_str.c, message + 1, 4);
            int message_length = message_length_str.i;            

            // Récupérer le nombre de messages
            union int_char message_count_str;
            strncpy(message_count_str.c, message + 5, 4);
            int message_count = message_count_str.i;

            // Récupérer le numéro de la boîte aux lettres
            union int_char mailbox_number_str;
            strncpy(mailbox_number_str.c, message + 9, 4);
            int mailbox_number = mailbox_number_str.i;

            // Afficher les informations récupérées
            printf("SERVER_SENDER: message_length=%d, message_count=%d, mailbox_number=%d\n", message_length, message_count, mailbox_number);

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
            union int_char mailbox_number_str;
            strncpy(mailbox_number_str.c, message + 1, 4);
            int mailbox_number = mailbox_number_str.i;

            // Afficher les informations récupérées
            printf("SERVER_RECEIVER: mailbox_number=%d\n", mailbox_number);

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