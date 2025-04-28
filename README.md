# Mailbox System (MBS)

## Description

This project implements a mailbox system in C that enables communication between multiple entities via a central server. Senders transmit messages to the server, which stores them in mailboxes associated with receivers. Receivers can then retrieve messages from their mailboxes.

## Features

- **Sender** : Sends messages to the server to be stored in a specific mailbox.
- **Receiver** : Retrieves messages from a mailbox via the server.
- **Server** : Manages connections from senders and receivers, and stores messages in mailboxes.
- **Mailbox Management** : Implementation of a linked list data structure to manage mailboxes and messages.

## Installation

To clone the project, use the following command:

```bash
git clone
```

## Compilation

To compile the project, use the provided `makefile`. The generated executable will be named `tsock`.

### Commands

- **Compile the project**:
  ```bash
  make
  ```
- **Clean generated files**:
  ```bash
  make clean
  ```

### Usage

The `tsock` executable should be run with the following arguments:

```bash
./tsock [-p|-s|-b|-e|-r] [-n nb_message] [-l len_message] [hostname] port
```

Options:

- `-p` : Sink mode (receiving messages).
- `-s` : Source mode (sending messages).
- `-b` : Server mode (mailbox).
- `-e` : Sender mode.
- `-r` : Receiver mode.
- `-n` : Number of messages to send or receive.
- `-l` : Length of messages.
- `hostname` : Hostname for client connections.
- `port` : Listening or connection port.

## Dependencies

- Standard C Library: `stdio.h`, `stdlib.h`, `string.h`, etc.
- Socket Library: `sys/socket.h`, `netinet/in.h`, `netdb.h`.

## Authors

- Aïman Al-SABAGH-NAHHAS
- Alain PIALLAT

## License

This project is licensed under the **Apache License 2.0**.\
See the [LICENSE](LICENSE) file for details.