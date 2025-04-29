#ifndef CLIENT_H
#define CLIENT_H

// Inclusion des bibliothèques nécessaire
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include "Utils.h"

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
int emitter(char *hostname, int port, int length, int count, int number);

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
int receiver(char *hostname, int port, int number);

#endif