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
git clone "https://github.com/AlainPiallat/tsock"
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

L'exécutable `tsock` doit être lancé avec la syntaxe suivante :

```bash
./tsock [MODES] [OPTIONS] [hostname] port
```

#### MODES (un seul obligatoire) :
- `-p`              Mode puits (réception de messages)
- `-s`              Mode source (envoi de messages)
- `-b`              Mode serveur (boîte aux lettres centrale)
- `-e bal_num`      Mode émetteur (envoi vers la boîte aux lettres numéro `bal_num`)
- `-r bal_num`      Mode récepteur (lecture de la boîte aux lettres numéro `bal_num`)
- `-h`              Affiche l'aide

#### OPTIONS :
- `-u`              Utilise le protocole UDP (par défaut : TCP)  
                    *(utilisé avec -p et -s uniquement)*
- `-n nb_message`   Nombre de messages à envoyer ou recevoir (défaut : 10)  
                    *(utilisé avec -p, -s, -e, -r)*
- `-l len_message`  Longueur des messages (défaut : 30)  
                    *(utilisé avec -p, -s, -e)*

#### ARGUMENTS :
- `hostname`        Nom d'hôte du serveur (requis pour source, émetteur, récepteur)
- `port`            Port d'écoute ou de connexion

#### EXEMPLES :
```bash
./tsock -p -n 5 -l 40 12345
./tsock -s -n 5 -l 40 localhost 12345
./tsock -b 12345
./tsock -e 2 localhost 12345
./tsock -r 2 localhost 12345
```

**Notes :**
- Un seul mode (`-p`, `-s`, `-b`, `-e`, `-r`) doit être spécifié à la fois.
- Utilisez `-h` pour afficher l'aide complète.

## Dépendances

- Bibliothèque standard C : `stdio.h`, `stdlib.h`, `string.h`, etc.
- Bibliothèque de sockets : `sys/socket.h`, `netinet/in.h`, `netdb.h`.

## Auteurs

- Aïman Al-SABAGH-NAHHAS
- Alain PIALLAT

## License

Ce projet est sous licence **Apache License 2.0**.\
Voir le fichier [LICENSE](LICENSE) pour plus de détails.