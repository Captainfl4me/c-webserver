#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <winsock2.h>

#include "http.h"

typedef struct SERVER {  
  SOCKET sock;
  SOCKADDR_IN sin;
} SERVER;

void configureServer(SERVER* server, char* addr, u_short port);
void listenningConnection(SERVER* server);
void freeServer(SERVER* server);

#endif