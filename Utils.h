#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

/**
 * @union int_char
 * @brief A union that allows interpreting an integer as an array of characters.
 *
 * This union provides a way to access the individual bytes of an integer
 * by treating it as a character array. It is useful for low-level operations
 * such as byte manipulation or serialization.
 *
 * Members:
 * - `i` : An integer value.
 * - `c` : A character array with a size equal to the size of an integer.
 */
union int_char {
    int i;
    char c[sizeof(int)];
};

/**
 * Convertit une chaîne de caractères en entier.
 * 
 * @param str La chaîne de caractères à convertir.
 * @return L'entier converti.
 */
int string_to_int(const char *str);

/**
 * Génère un message au format spécifique.
 * 
 * Le message suit le format suivant :
 * <numéro> <lettre répétée>
 * où le numéro de message est un entier de 5 chiffres et la lettre est déterminée par le numéro modulo 26.
 * 
 * @param n Le numéro de message.
 * @param N La longueur totale du message.
 * @return Un pointeur vers la chaîne de caractères contenant le message généré.
 */
char* genere_message(int n, int N);

/**
 * Construit une adresse locale pour le socket.
 * 
 * @param port Le port à utiliser pour l'adresse locale.
 * @param addr Pointeur vers la structure sockaddr_in où l'adresse sera stockée.
 * @return 0 en cas de succès, -1 en cas d'erreur.
 */
int construire_adresse_local(int port, struct sockaddr_in* addr);

/**
 * Construit une adresse de destination pour le socket.
 * 
 * @param dest Pointeur vers la structure hostent contenant les informations sur l'hôte de destination.
 * @param port Le port à utiliser pour l'adresse de destination.
 * @param addr Pointeur vers la structure sockaddr_in où l'adresse sera stockée.
 * @return 0 en cas de succès, -1 en cas d'erreur.
 */
int construire_adresse_dest(struct hostent *dest, int port, struct sockaddr_in* addr);

#endif