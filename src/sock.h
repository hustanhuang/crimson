#ifndef SOCK_H
#define SOCK_H

#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct sockaddr SA;
typedef struct sockaddr_in SA_in;

extern int retrieve_listenfd();
extern void handle_accept(int kqfd, int client_fd);
extern void handle_read(int kqfd, int client_fd);
extern void handle_write(int kqfd, int client_fd);

#endif
