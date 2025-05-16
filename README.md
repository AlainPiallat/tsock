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
git clone "https://github.com/AlainPiallat/tsock"
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

The `tsock` executable should be run with the following syntax:

```bash
./tsock [MODES] [OPTIONS] [hostname] port
```

#### MODES (only one required):
- `-p`              Sink mode (receiving messages)
- `-s`              Source mode (sending messages)
- `-b`              Server mode (central mailbox)
- `-e bal_num`      Sender mode (send to mailbox number `bal_num`)
- `-r bal_num`      Receiver mode (read from mailbox number `bal_num`)
- `-h`              Display this help message

#### OPTIONS:
- `-u`              Use UDP protocol (default: TCP)  
                    *(used only with -p and -s)*
- `-n nb_message`   Number of messages to send or receive (default: 10)  
                    *(used with -p, -s, -e, -r)*
- `-l len_message`  Length of messages (default: 30)  
                    *(used with -p, -s, -e)*

#### ARGUMENTS:
- `hostname`        Server hostname (required for source, sender, receiver)
- `port`            Listening or connection port

#### EXAMPLES:
```bash
./tsock -p -n 5 -l 40 12345
./tsock -s -n 5 -l 40 localhost 12345
./tsock -b 12345
./tsock -e 2 localhost 12345
./tsock -r 2 localhost 12345
```

**Notes:**
- Only one mode (`-p`, `-s`, `-b`, `-e`, `-r`) should be specified at a time.
- Use `-h` to display the full help message.

## Dependencies

- Standard C Library: `stdio.h`, `stdlib.h`, `string.h`, etc.
- Socket Library: `sys/socket.h`, `netinet/in.h`, `netdb.h`.

## Authors

- Aïman Al-SABAGH-NAHHAS
- Alain PIALLAT

## License

This project is licensed under the **Apache License 2.0**.\
See the [LICENSE](LICENSE) file for details.