#include "server.h"

void configureServer(SERVER* server, char* addr, u_short port){
  server->sock = socket(AF_INET,SOCK_STREAM,0);
  if (server->sock == INVALID_SOCKET) {
      wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
      exit(EXIT_FAILURE);
  }
  
  server->sin.sin_addr.s_addr = inet_addr(addr);
  server->sin.sin_family = AF_INET;
  server->sin.sin_port = htons(port);

  if(bind(server->sock, (SOCKADDR *)&server->sin, sizeof(server->sin)) < 0){
    wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
    exit(EXIT_FAILURE);
  }
}

void listenningConnection(SERVER* server){
  HTTP_Request* request = Http_Request_new();

  listen(server->sock, 0);
  printf("Server listening...\n");
  while(1){
    SOCKET request_socket = accept(server->sock, NULL, NULL);
    if(request_socket != INVALID_SOCKET)
    {
      parseHTTPRequest(&request_socket, request);
      printf("Complete data: %d \n", (int)request->buffer_size);
      printf("%.*s\n", (int)request->buffer_size, request->buffer);
    
      HTTP_Response* response = Http_Response_new();

      if(request->info.type == GET){
        
        int filePathLength = snprintf(NULL, 0, "www/%.*s", request->info.path_size, request->info.path);
        if(filePathLength > 0) {
          char* filePath = (char*) malloc(filePathLength*sizeof(char));
          sprintf(filePath, "www/%.*s", request->info.path_size, request->info.path); 

          FILE *f = fopen(filePath, "rb");

          if (f == NULL) {
            printf("Ressouces not found!\n");
            response->status_code = 404;
          } else{
            fseek(f, 0, SEEK_END);
            long fsize = ftell(f);
            fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

            response->buffer_size = fsize + 1;
            response->buffer = malloc(response->buffer_size*sizeof(char));
            fread(response->buffer, fsize, 1, f);

            setHeader(response, "Content-Type", "text/html; charset=UTF-8");
            
            int contentLengthSize = snprintf(NULL, 0, "%d", response->buffer_size);
            if(contentLengthSize > 0) {
              char* str = (char*) malloc(contentLengthSize*sizeof(char));
              sprintf(str, "%d", response->buffer_size); 
              setHeader(response, "Content-Length", str);
              response->status_code = 200;
            }

            fclose(f);
            printf("close file!\n");
          }
        }
        
      }else{
        response->status_code = 200;
      }
      setHeader(response, "Server", "cserver/0.0.1");
      setHeader(response, "Connection", "close");

      size_t response_size;
      char* responseBuffer = writeResponse(response, &response_size);

      printf("Response data: %d \n", (int)response_size);
      printf("%.*s\n", (int)response_size, responseBuffer);

      send(request_socket, responseBuffer, (int)response_size, 0);
      closesocket(request_socket);
      
      freeHTTPRequest(request);
      freeHTTPResponse(response);
    }
  }

  free(request);
}

void freeServer(SERVER* server) {
  closesocket(server->sock);
}