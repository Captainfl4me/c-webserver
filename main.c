#include <stdio.h>
#include <sys/types.h>
#include <winsock2.h>

#include "server.h"

int main()
{
  WSADATA WSAData;
  WSAStartup(MAKEWORD(2, 0), &WSAData);

  SERVER server;
  configureServer(&server, "127.0.0.1", 4148);

  listenningConnection(&server);

  freeServer(&server);
  WSACleanup();
  return 0;
}

