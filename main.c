#include <stdio.h>
#include <sys/types.h>
#include <winsock2.h>

#include "http.h"

void configureSocket(SOCKET* sock);
void configureAddr(SOCKADDR_IN* sin);
void runServer(SOCKET* sock, SOCKADDR* sin, unsigned long long sizeof_sin);

int main()
{
  WSADATA WSAData;
  WSAStartup(MAKEWORD(2, 0), &WSAData);

  SOCKET sock;
  SOCKADDR_IN sin;
  configureSocket(&sock);
  configureAddr(&sin);

  if(bind(sock, (SOCKADDR *)&sin, sizeof(sin)) < 0){
        printf("Cannot bind socket!");
        return 1;    
  }
  runServer(&sock, (SOCKADDR *)&sin, sizeof(sin));

  closesocket(sock);
  WSACleanup();
  return 0;
}

void configureSocket(SOCKET* sock){
  *sock = socket(AF_INET,SOCK_STREAM,0);
}
void configureAddr(SOCKADDR_IN* sin){
  (*sin).sin_addr.s_addr = inet_addr("127.0.0.1");
  (*sin).sin_family = AF_INET;
  (*sin).sin_port = htons(4148);
}

void runServer(SOCKET* sock, SOCKADDR* sin, unsigned long long sizeof_sin){
  HTTP_Request* request = Http_Request_new();

  listen(*sock, 0);
  printf("Server listening...\n");
  while(1){
    int sizeof_csin = sizeof(*sin);
    SOCKET val = accept(*sock, sin, &sizeof_csin);
    if(val != INVALID_SOCKET)
    {
      parseHTTPRequest(&val, request);
      printf("Complete data: %d \n", (int)request->buffer_size);
      printf("%.*s\n", (int)request->buffer_size, request->buffer);
    
      HTTP_Response* response = Http_Response_new();
      response->status_code = 200;
      setHeaders(response, *createHeader("Server", "cserver/0.0.1"));
      setHeaders(response, *createHeader("Connection", "close"));

      size_t response_size;
      char* responseBuffer = writeResponse(response, &response_size);

      printf("Response data: %d \n", (int)response_size);
      printf("%.*s\n", (int)response_size, responseBuffer);

      send(val, responseBuffer, (int)response_size, 0);
      closesocket(val);
      freeHTTPRequest(request);
      freeHTTPResponse(response);
    }
  }

  free(request);
}
