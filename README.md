# TCP Forum

## Description

This project is a TCP forum written in C. It uses a client–server architecture:

* The server accepts multiple TCP connections and handles the messages.
* The client connects to the server and allows users to interact with the forum.

---

## Files

```
client_tcp_forum.c  // Client code
serveur_tcp_forum.c // Server code
user.c              // Functions that allow user and message creation
Makefile            // Compilation
README.md           // Documentation
```

---

## Requirements

* Linux or Unix system
* gcc compiler
* make

---

## Compile

From the project directory, run:

```bash
make
```

This will generate:

* `serveur_tcp_forum`
* `client_tcp_forum`

To clean the project:

```bash
make clean
```

---

## Run

### Start the server

```bash
./serveur_tcp_forum
```

The server listens on port 9600.

### Start the client

In another terminal:

```bash
./client_tcp_forum <server_address>
```

Example:

```bash
./client_tcp_forum localhost
```

---

## Notes

* Communication is done using TCP sockets.
* Buffer size is fixed (200 bytes).
* The protocol logic is implemented directly in the source code.

---
