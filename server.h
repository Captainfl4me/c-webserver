#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <winsock2.h>
#include <Ws2tcpip.h>

#include "http.h"
#include "path_url.h"

typedef struct SERVER {  
  SOCKET sock;
  SOCKADDR_IN sin;
  unsigned int endpointsLength;
  URL_PATH* endpoints;
} SERVER;

unsigned int resolveUrlPath(SERVER* server, char* full_path, int path_len);
void configureServer(SERVER* server, char* addr, u_short port);
void listenningConnection(SERVER* server);
void freeServer(SERVER* server);

void createEndpoint(SERVER* server, URL_PATH* path, enum HTTP_Type http_Type, void (*callback)(HTTP_Request*, HTTP_Response*));
void createRedirection(SERVER* server, URL_PATH* path, char* path_dst);
void createStaticEndpoint(SERVER* server, URL_PATH* path, char* path_dst);

#endif