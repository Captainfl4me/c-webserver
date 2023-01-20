#include <stdio.h>
#include <sys/types.h>
#include <winsock2.h>

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
  char buff[256];

  listen(*sock, 0);
  printf("Server listening...\n");
  while(1){
    int sizeof_csin = sizeof(*sin);
    SOCKET val = accept(*sock, sin, &sizeof_csin);
    if(val != INVALID_SOCKET)
    {
      int data_len = recv(val, buff, 256, MSG_PEEK);
      while(data_len >= 0){
        printf("Receive data: %d \n", data_len);
        printf("%s", buff);
      }
    
      closesocket(val);
    }
  }
}
