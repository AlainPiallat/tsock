# Système de Boîte aux Lettres (BAL)

## Description

Ce projet implémente un système de boîte aux lettres en C permettant la communication entre plusieurs entités via un serveur central. Les émetteurs envoient des messages au serveur, qui les stocke dans des boîtes aux lettres associées à des récepteurs. Les récepteurs peuvent ensuite récupérer les messages depuis leurs boîtes aux lettres.

## Fonctionnalités

- **Émetteur (Sender)** : Envoie des messages au serveur pour les stocker dans une boîte aux lettres spécifique.
- **Récepteur (Receiver)** : Récupère les messages depuis une boîte aux lettres via le serveur.
- **Serveur** : Gère les connexions des émetteurs et des récepteurs, et stocke les messages dans des boîtes aux lettres.
- **Gestion des boîtes aux lettres** : Implémentation d'une structure de données en liste chaînée pour gérer les boîtes aux lettres et les messages.

## Installation

Pour cloner le projet, utilisez la commande suivante :

```bash
git clone
```

## Compilation

Pour compiler le projet, utilisez le fichier `makefile` fourni. L'exécutable généré s'appellera `tsock`.

### Commandes

- **Compiler le projet** :
  ```bash
  make
  ```
- **Nettoyer les fichiers générés** :
  ```bash
  make clean
  ```
  
### Utilisation

L'exécutable `tsock` doit être lancé avec les arguments suivants :

```bash
./tsock [-p|-s|-b|-e|-r] [-n nb_message] [-l len_message] [hostname] port
```

Options :

- `-p` : Mode puits (réception de messages).
- `-s` : Mode source (envoi de messages).
- `-b` : Mode serveur (boîte aux lettres).
- `-e` : Mode émetteur.
- `-r` : Mode récepteur.
- `-n` : Nombre de messages à envoyer ou recevoir.
- `-l` : Longueur des messages.
- `hostname` : Nom d'hôte pour les connexions client.
- `port` : Port d'écoute ou de connexion.

## Dépendances

- Bibliothèque standard C : `stdio.h`, `stdlib.h`, `string.h`, etc.
- Bibliothèque de sockets : `sys/socket.h`, `netinet/in.h`, `netdb.h`.

## Auteurs

- Aïman Al-SABAGH-NAHHAS
- Alain PIALLAT

## License

Ce projet est sous licence **Apache License 2.0**.\
Voir le fichier [LICENSE](LICENSE) pour plus de détails.