/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>
/* contient les fonctions diverses nécessaires au programme */
#include "Utils.h"
/* code du serveur */
#include "bal.h"
/* code des client */
#include "client.h"
/* autre librairie */
#include <stdio.h>

int puit_UDP_aff(int port, int taille_max, int n) {
	// Créer un socket UDP 
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	 
	// Construire l'adresse locale pour le serveur
	struct sockaddr_in* adresse_local = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	
	construire_adresse_local(port, adresse_local);
	 
	// Lier le socket à l'adresse locale 
	bind(sock, adresse_local, sizeof(struct sockaddr_in));
	
	// informe de l'ouverture du puit
	if (n != -1)
		printf("PUITS: lg_mesg-lu=%d, port=%d, nb_receptions=%d, TP=udp\n", taille_max, port, n);
	else
		printf("PUITS: lg_mesg-lu=%d, port=%d, nb_receptions=infini, TP=udp\n", taille_max, port);

	// Boucle principale pour recevoir des messages 
	int nb_receptions = 1;
	while (1)
	{
		// Lire un message du client 
		struct sockaddr* adresse_expediteur = (struct sockaddr*) malloc(sizeof(struct sockaddr));
		socklen_t taille_expediteur;
		char message[taille_max];
		if(recvfrom(sock, message, taille_max, 0, adresse_expediteur, &taille_expediteur) != -1) {
			printf("PUITS: Reception n°%d (%ld) [%s]\n", nb_receptions, strlen(message), message);

			// Incrémenter le nombre de réceptions
			nb_receptions++;

			// Vérifier si le nombre de réceptions est atteint
			if (n != -1 && nb_receptions >= n) {
				break;
			}
		}
		
	}

	printf("PUITS: fin\n");
	 
	// Fermer le socket après utilisation #TODO question prof
	close(sock);
	return 0;
}

int source_UDP_aff(char* dest, int port, int n, int l) {
	// Créer un socket UDP 
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	
	// Obtenir les informations de l'hôte à partir du nom de destination 
	struct hostent* host_info = gethostbyname(dest);
	
	// Construire l'adresse du destinataire 
	struct sockaddr_in* adresse_dest = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	
	construire_adresse_dest(host_info, port, adresse_dest);
	
	// informe de l'ouverture de la source
	if (n == -1) n = 10;
	printf("SOURCE: lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=udp, dest=%s\n", l, port, n, dest);
	// Envoye des messages un par un
	for (int i = 1; i < n+1; i++)
	{
		// Générer un message à envoyer 
		char* message = genere_message(i, l);
		
		// Obtenir la taille du message 
		int taille_message = strlen(message);

		// Informer de l'envoi du message
		printf("SOURCE: Envoi n°%d (%d) [%s]\n", i, taille_message, message);

		// Envoyer le message 
		sendto(sock, message, taille_message, 0, adresse_dest, sizeof(struct sockaddr_in)); 
	}
	
	// Informer de la fin de l'envoi
	printf("SOURCE: fin\n");
	
	// Fermer le socket après utilisation 
	close(sock);
	return 0;
}

int thread_puit_TCP_aff(int sock, int taille_max, int n) {
	// Boucle principale pour recevoir des messages 
	while (1)
	{
		// Lire un message du client 
		char message[taille_max];
		int bytes_received = recv(sock, message, taille_max, 0);
		if (bytes_received > 0) {
			// Afficher le message reçu
			printf("PUITS: Reception n°%d (%d) [%s]\n", 1, bytes_received, message);
			n--;
			if (n == 0) {
				shutdown(sock, 2); // TODO à gérer coté source
				break;
			}
		} else if (bytes_received == 0) {
			// Connexion fermée par le client
			printf("PUITS: connexion fermée par le client\n");
			break;
		} else {
			// Erreur lors de la réception
			perror("PUITS: erreur lors de la réception");
			break;
		}
	}
	// Informer de la fin de la communication
	printf("PUITS: fin\n");
	
	// Fermer le socket après la fin de la communication
	close(sock);
	return 0;
}
	
int puit_TCP_aff(int port, int taille_max, int n) {
	// Créer un socket TCP 
	int sock = socket(AF_INET,SOCK_STREAM,0);	
	
	// Construire l'adresse locale pour le serveur 
	struct sockaddr_in* adresse_local = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	
	construire_adresse_local(port, adresse_local);
	
	// Lier le socket à l'adresse locale 
	bind(sock, adresse_local, sizeof(struct sockaddr_in));
	
	// Mettre le socket en écoute pour accepter les connexions entrantes 
	listen(sock, 5);

	// informe de l'ouverture du puit
	if (n != -1)
		printf("PUITS: lg_mesg-lu=%d, port=%d, nb_receptions=%d, TP=tcp\n", taille_max, port, n);
	else
		printf("PUITS: lg_mesg-lu=%d, port=%d, nb_receptions=infini, TP=tcp\n", taille_max, port);
	
	// Boucle principale pour accèpter les connexions 
	while (1)
	{
		struct sockaddr_in* adresse_source = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
		socklen_t taille_source = sizeof(struct sockaddr_in);
		// Accèpter une nouvelle connexion 
		int sock_alloue = accept(sock, adresse_source, &taille_source);
		if (sock_alloue != -1) {
			// Informer de la connexion
			printf("PUITS: connexion acceptée\n");
			// Gérer la connexion dans un thread ou un processus séparé 
			thread_puit_TCP_aff(sock_alloue, taille_max, n);
		}
	
	}

	// Fermer le socket principal 
	close(sock);
	return 0;
}

int source_TCP_aff(char* dest, int port, int n, int l) {
	// Créer un socket TCP 
	int sock = socket(AF_INET,SOCK_STREAM,0);
	
	// Obtenir les informations de l'hôte à partir du nom de destination 
	struct hostent* host_info = gethostbyname(dest);
	
	// Construire l'adresse du destinataire 
	struct sockaddr_in* adresse_dest = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	
	construire_adresse_dest(host_info, port, adresse_dest);

	// informe de l'ouverture de la source
	if (n == -1) n = 10;
	printf("SOURCE: lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=tcp, dest=%s\n", l, port, n, dest);
	
	// Tenter d'établir la connexion jusqu'à ce qu'elle réussisse
	int nb_essais = 0;
	while (connect(sock, adresse_dest, sizeof(struct sockaddr_in)) != 0) {
		nb_essais++;
		sleep(1);
		if (nb_essais > 3) {
			perror("SOURCE: connexion");
			exit(111);
		}
	}

	// informe de la connexion
	printf("SOURCE: connecté\n");

	for (int i = 1; i < n+1; i++)
	{
		// Générer un message à envoyer 
		char* message = genere_message(i, l);
		
		// Obtenir la taille du message 
		int taille_message = strlen(message);

		// Informer de l'envoi du message
		printf("SOURCE: Envoi n°%d (%d) [%s]\n", i, taille_message, message);

		// Envoyer le message 
		nb_essais = 0;
		while (send(sock, message, taille_message, 0) == -1) {
			nb_essais++;
			sleep(1);
			if (nb_essais > 3) {
				perror("SOURCE: envoi");
				exit(111);
			}
		}
	}
	
	nb_essais = 0;
	while (shutdown(sock, 2) == -1) {
		nb_essais++;
		sleep(1);
		if (nb_essais > 3) {
			perror("SOURCE: fermeture");
			exit(111);
		}
	}

	// informe de la fin de l'envoi
	printf("SOURCE: fin\n");

	// Fermer le socket après utilisation 
	close(sock);
	return 0;
}

int main(int argc, char **argv) {
    int c;
    extern char *optarg;
    extern int optind;

    int nb_message = 10;  	// Nombre de messages à envoyer ou à recevoir
    int len_message = 30; 	// Taille des messages
	int bal_num = -1;		// Numéro de la boîte aux lettres
    int mode = -1;        	// 0=puits, 1=source, 2=boite aux lettres, 3=émetteur, 4=récepteur
    int port = -1;        	// Port du serveur
	int protocol = 0;		// Protocole (0=TCP, 1=UDP)
    char *hostname = NULL; 	// Nom d'hôte pour les connexions client

    // Analyse des arguments
    while ((c = getopt(argc, argv, "psun:l:be:r:h")) != -1) {
        switch (c) {
            case 'p':
                if (mode != -1) {
                    printf("Erreur : mode déjà défini.\n");
                    exit(1);
                }
                mode = 0; // Mode puits
                break;

            case 's':
                if (mode != -1) {
                    printf("Erreur : mode déjà défini.\n");
                    exit(1);
                }
                mode = 1; // Mode source
                break;

			case 'u':
				int protocol = 1; // Protocole UDP
				break;

            case 'b':
                if (mode != -1) {
                    printf("Erreur : mode déjà défini.\n");
                    exit(1);
                }
                mode = 2; // Mode boîte aux lettres (serveur)
                break;

            case 'e':
                if (mode != -1) {
                    printf("Erreur : mode déjà défini.\n");
                    exit(1);
                }
                mode = 3; // Mode émetteur
				if (optarg != NULL) {
					bal_num = atoi(optarg); // Numéro de la boîte aux lettres
				} else {
					printf("Erreur : numéro de boîte aux lettres requis pour le mode émetteur.\n");
					exit(1);
				}
                break;

            case 'r':
                if (mode != -1) {
                    printf("Erreur : mode déjà défini.\n");
                    exit(1);
                }
                mode = 4; // Mode récepteur
				if (optarg != NULL) {
					bal_num = atoi(optarg); // Numéro de la boîte aux lettres
				} else {
					printf("Erreur : numéro de boîte aux lettres requis pour le mode récepteur.\n");
					exit(1);
				}
                break;

            case 'n':
                nb_message = atoi(optarg);
                break;

            case 'l':
                len_message = atoi(optarg);
                break;

			case 'h':
				// Afficher le manuel d'utilisation (help.txt)
				FILE *file = fopen("help.txt", "r");
				if (file == NULL) {
					perror("Erreur lors de l'ouverture du fichier d'aide");
					exit(1);
				}
				char line[256];
				while (fgets(line, sizeof(line), file) != NULL) {
					printf("%s", line);
				}
				fclose(file);
				exit(0);
				break;

            default:
                printf("Usage : %s [-p|-s|-b|-e bal_num|-r bal_num|-h] [-u] [-n nb_message] [-l len_message] [hostname] port\n", argv[0]);
				printf("Pour plus d'informations : %s -h\n", argv[0]);
                exit(1);
        }
    }

    // Vérification des arguments restants
    if (optind < argc - 1) {
        hostname = argv[optind];
    }
    port = string_to_int(argv[argc - 1]);

    if (mode == -1 || port == -1) {
        printf("Usage : %s [-p|-s|-b|-e bal_num|-r bal_num|-h] [-u] [-n nb_message] [-l len_message] [hostname] port\n", argv[0]);
		printf("Pour plus d'informations : %s -h\n", argv[0]);
        exit(1);
    }

    // Exécution en fonction du mode
    switch (mode) {
		case 0: // Mode puits
			if (protocol == 1) {
				printf("Mode : Puits (UDP)\n");
				puit_UDP_aff(port, len_message, nb_message);
			} else {
				printf("Mode : Puits (TCP)\n");
				puit_TCP_aff(port, len_message, nb_message);
			}
            break;

        case 1: // Mode source
            if (hostname == NULL) {
                printf("Erreur : le nom d'hôte est requis pour le mode source.\n");
                exit(1);
            }
			if (protocol == 1) {
				printf("Mode : Source (UDP)\n");
				source_UDP_aff(hostname, port, nb_message, len_message);
			} else {
				printf("Mode : Source (TCP)\n");
				source_TCP_aff(hostname, port, nb_message, len_message);
			}
			break;

        case 2: // Mode boîte aux lettres (serveur)
            printf("Mode : Serveur (Boîte aux lettres)\n");
            server(port);
            break;

        case 3: // Mode émetteur
            printf("Mode : Émetteur\n");
            if (hostname == NULL) {
                printf("Erreur : le nom d'hôte est requis pour le mode émetteur.\n");
                exit(1);
            }
            emitter(hostname, port, len_message, nb_message, bal_num);
            break;

        case 4: // Mode récepteur
            printf("Mode : Récepteur\n");
            if (hostname == NULL) {
                printf("Erreur : le nom d'hôte est requis pour le mode récepteur.\n");
                exit(1);
            }
            receiver(hostname, port, bal_num);
            break;

        default:
            printf("Erreur : mode inconnu.\n");
            exit(1);
    }

    return 0;
}



