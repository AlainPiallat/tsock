#ifndef BAL_H
#define BAL_H

// Inclusion des bibliothèques nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include "Utils.h"
#include "chained_list.h"

// Fonction


/**
/**
 * Cette fonction récupère les messages envoyés par l'émetteur via le socket qui lui est dédié 
 * et les ajoute à la boîte aux lettres. Une fois tous les messages reçus, elle ferme le socket.
 *
 * @param sock Socket dédié à la communication avec l'émetteur.
 * @param mbox Pointeur vers la boite aux lettres destinataire.
 * @param length Longueur des messages reçus.
 * @param count Nombre de messages reçus.
 * @return Pointeur générique (void *) Renvoie 0 si tout s'est bien passé, -1 en cas d'erreur.
 */
int server_sender(int sock, mailbox *mbox, int length, int count);

/**
 * Cette fonction envoie les messages de la boîte aux lettres vers le récepteur via le socket qui lui est dédié.
 * Une fois tous les messages envoyés, elle ferme le socket.
 * 
 * @param sock Socket dédié à la communication avec le récepteur.
 * @param mbox Pointeur vers la boîte aux lettres du récepteur.
 * @return Pointeur générique (void *) Renvoie 0 si tout s'est bien passé, -1 en cas d'erreur.
 */
int server_receiver(int sock, mailbox *mbox);

/**
 * Fonction principale du serveur qui gère les connexions entrantes et créer les sockets dédiés pour chaque client ainsi que les boîtes aux lettres.
 * Une fois la connexion établie, elle attend les messages d'initialisation du client pour créer la boîte aux lettres correspondante.
 * Ensuite, elle créer un thread pour gérer la communication et appelle la fonction d'envoi ou de réception selon le message reçu.
 * 
 * @param port Port d'écoute du serveur.
 * @return 0 si le serveur a réussi, -1 sinon.
 */
int server(int port);



#endif 