#ifndef CHAINED_LIST_H
#define CHAINED_LIST_H

#include <stdlib.h>
#include <string.h>

// Structures de données
typedef struct message {
    char *content;
    int length;
    struct message *next;
} message;

typedef struct mailbox {
    int number;
    struct message *messages;
    struct mailbox *next;
} mailbox;

// Fonctions

/**
 * Initialise la liste chaînée de boites aux lettres.
 * 
 * @param mailboxes Pointeur vers l'adresse où la liste chaînée sera stockée.
 * @return 0 si l'initialisation a réussi, -1 sinon.
 */

int initialize_mailbox(mailbox **mailboxes);

/**
 * Récupère la boite aux lettres correspondant au numéro fourni en paramètre.
 * Si la boite aux lettres n'existe pas, elle est créée.
 * 
 * @param mailboxes Pointeur vers la liste chaînée de boites aux lettres.
 * @param number Numéro de la boite aux lettres à récupérer.
 * @return Pointeur vers la boite aux lettres.
 */
mailbox *get_mailbox(mailbox **mailboxes, int number);

/**
 * Libère la mémoire allouée pour les boites aux lettres.
 * 
 * @param mailboxes Pointeur vers la liste chaînée de boites aux lettres.
 */
void free_mailboxes(mailbox **mailboxes);

/**
 * Ajoute un message à la boite aux lettres fournie en paramètre.
 * 
 * @param mail_box Pointeur vers la boite aux lettres.
 * @param message Message à ajouter à la boite aux lettres.
 * @param length Longueur du message.
 * @return 0 si l'ajout a réussi, -1 sinon.
 */
int add_message(mailbox *mail_box, char *msg, int length);

/**
 * Récupère le message suivant de la boite aux lettres fournie en paramètre.
 * La fonction libère la mémoire du message récupéré.
 * 
 * @param mail_box Pointeur vers la boite aux lettres.
 * @param length Pointeur vers la longueur du message.
 * @return Pointeur vers le message, ou NULL si la boite aux lettres est vide.
 */
char *get_message(mailbox *mail_box, int *length);


#endif // CHAINED_LIST_H