#include "chained_list.h"

int initialize_mailbox(mailbox **mailboxes) {
    *mailboxes = NULL;
    return 0;
}

mailbox *get_mailbox(mailbox **mailboxes, int number) {
    // Vérifier si la liste des boites aux lettres est vide
    if (*mailboxes == NULL) {
        // La liste est vide, on crée la première boite aux lettres
        printf("Création de la boite aux lettres %d\n", number);
        mailbox *new_mailbox = (mailbox *)malloc(sizeof(mailbox));
        new_mailbox->number = number;
        new_mailbox->messages = NULL;
        new_mailbox->next = NULL;
        *mailboxes = new_mailbox; // Mettre à jour la liste
        return new_mailbox; // Retourner la nouvelle boite aux lettres
    }

    mailbox *current = *mailboxes;
    mailbox *previous = NULL;

    // Parcourir la liste pour trouver la boite aux lettres
    while (current != NULL) {
        if (current->number == number) {
            return current; // La boite aux lettres a été trouvée
        }
        previous = current;
        current = current->next;
    }

    // Si la boite aux lettres n'existe pas, on la crée
    printf("Création de la boite aux lettres %d\n", number);
    mailbox *new_mailbox = (mailbox *)malloc(sizeof(mailbox));
    new_mailbox->number = number;
    new_mailbox->messages = NULL;
    new_mailbox->next = NULL;
    previous->next = new_mailbox; // Ajouter la nouvelle boite aux lettres à la fin de la liste

    return new_mailbox;
}

void free_mailboxes(mailbox **mailboxes) {
    mailbox *current = *mailboxes;
    mailbox *next;

    while (current != NULL) {
        // Libérer les messages de cette boite
        message *msg = current->messages;
        while (msg != NULL) {
            message *next_msg = msg->next;
            free(msg->content);
            free(msg);
            msg = next_msg;
        }

        next = current->next;
        free(current);
        current = next;
    }

    *mailboxes = NULL;
}

int add_message(mailbox *mail_box, char *msg, int length) {
    // Créer un nouveau message
    message *new_message = (message *)malloc(sizeof(message));
    new_message->content = (char *)malloc(length + 1);
    strncpy(new_message->content, msg, length);
    new_message->content[length] = '\0'; // Ajouter le caractère de fin de chaîne
    new_message->length = length;
    new_message->next = NULL;

    // Ajouter le message à la fin de la liste des messages
    if (mail_box->messages == NULL) {
        mail_box->messages = new_message; // La liste était vide
    } else {
        message *current = mail_box->messages;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_message; // Ajouter à la fin de la liste
    }

    return 0;
}

char *get_message(mailbox *mail_box, int *length) {
    if (mail_box->messages == NULL) {
        return NULL; // La boite aux lettres est vide
    }

    // Récupérer le premier message
    message *msg = mail_box->messages;
    char *content = msg->content;
    *length = msg->length;

    // Libérer la mémoire du message
    mail_box->messages = msg->next;
    free(msg);

    return content; // Retourner le message
}

void print_mailboxes(mailbox *mailboxes) {
    if (mailboxes == NULL) {
        printf("Aucune boite aux lettres.\n");
        return;
    }

    mailbox *current_mailbox = mailboxes;
    while (current_mailbox != NULL) {
        printf("Boite aux lettres %d:\n", current_mailbox->number);

        message *current_message = current_mailbox->messages;
        if (current_message == NULL) {
            printf("  Aucun message.\n");
        } else {
            int message_index = 1;
            while (current_message != NULL) {
                printf("  Message %d: %s (longueur: %d)\n", message_index, current_message->content, current_message->length);
                current_message = current_message->next;
                message_index++;
            }
        }

        current_mailbox = current_mailbox->next;
    }
}