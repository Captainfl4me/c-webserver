#include "server.h"

void configureServer(SERVER* server, char* addr, u_short port){
  server->sock = socket(AF_INET,SOCK_STREAM,0);
  if (server->sock == INVALID_SOCKET) {
      wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
      exit(EXIT_FAILURE);
  }

  inet_pton(AF_INET, addr, &(server->sin.sin_addr.s_addr));
  server->sin.sin_family = AF_INET;
  server->sin.sin_port = htons(port);

  if(bind(server->sock, (SOCKADDR *)&server->sin, sizeof(server->sin)) < 0){
    wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
    exit(EXIT_FAILURE);
  }

  server->endpointsLength = 0;
  server->endpoints = NULL;
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

      int index_endpointToExecute = resolveUrlPath(server, request->info.path, (int)request->info.path_size);
      if(index_endpointToExecute >= 0){
        switch (server->endpoints[index_endpointToExecute].type)
        {
        case ENDPOINT:
          server->endpoints[index_endpointToExecute].callback(request, response);
          response->status_code = 200;
          break;
          
        case REDIRECTION:
          setHeader(response, "location", server->endpoints[index_endpointToExecute].redirect_link);
          response->status_code = 302;
          break;
        
        default:
          break;
        }
      }else{
        response->status_code = 404;
      }

/*
      if(request->info.type == GET){
        
        int filePathLength = snprintf(NULL, 0, "www/%.*s", (int)request->info.path_size, request->info.path);
        if(filePathLength > 0) {
          char* filePath = (char*) malloc(filePathLength*sizeof(char));
          sprintf_s(filePath, filePathLength, "www/%.*s", (int)request->info.path_size, request->info.path); 

          FILE *f;
          errno_t err = fopen_s(&f, filePath, "rb");

          if (f == NULL) {
            printf("Ressouces not found!\n");
            response->status_code = 404;
          } else{
            fseek(f, 0, SEEK_END);
            long fsize = ftell(f);
            fseek(f, 0, SEEK_SET);  // same as rewind(f); 

            response->buffer_size = fsize + 1;
            response->buffer = malloc(response->buffer_size*sizeof(char));
            fread(response->buffer, fsize, 1, f);

            setHeader(response, "Content-Type", "text/html; charset=UTF-8");
            
            int contentLengthSize = snprintf(NULL, 0, "%d", response->buffer_size);
            if(contentLengthSize > 0) {
              char* str = (char*) malloc(contentLengthSize*sizeof(char));
              sprintf_s(str, contentLengthSize, "%d", response->buffer_size); 
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
*/
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
      free(response);
    }
  }

  free(request);
}

unsigned int resolveUrlPath(SERVER* server, char* full_path, int path_len){
  URL_PATH* requested_path = parseUrlPathToStruct(full_path, path_len);
  
  unsigned int full_path_len = 0;
  char *full_path_str = printUrlPath(requested_path, &full_path_len);
  printf("New request: %.*s\n", (int)full_path_len, full_path_str);
  
  unsigned int i;
  for(i = 0; i < server->endpointsLength; i++) {
    if(urlPathCmp(&server->endpoints[i], requested_path) == true) break;
  }
  if(i >= server->endpointsLength) i = -1;

  free(requested_path);
  return i;
}

void createEndpoint(SERVER* server, URL_PATH* path, enum HTTP_Type http_Type, void (*callback)(HTTP_Request*, HTTP_Response*)) {
  if(server->endpointsLength == 0)
    server->endpoints = (URL_PATH*) malloc((++server->endpointsLength) * sizeof(URL_PATH));
  else
    server->endpoints = (URL_PATH*) realloc(server->endpoints, (++server->endpointsLength) * sizeof(URL_PATH));

  path->type = ENDPOINT;
  path->callback = callback;
  server->endpoints[server->endpointsLength - 1] = *path;

  unsigned int full_path_len = 0;
  char *full_path = printUrlPath(path, &full_path_len);
  printf("Creating new endpoint: %.*s\n", (int)full_path_len, full_path);

  free(full_path);
}

void createRedirection(SERVER* server, URL_PATH* path, char* path_dst) {
  if(server->endpointsLength == 0)
    server->endpoints = (URL_PATH*) malloc((++server->endpointsLength) * sizeof(URL_PATH));
  else
    server->endpoints = (URL_PATH*) realloc(server->endpoints, (++server->endpointsLength) * sizeof(URL_PATH));

  path->type = REDIRECTION;
  path->callback = NULL;
  size_t link_len = strlen(path_dst)+1;
  path->redirect_link = (char*) malloc(link_len);
  strcpy_s(path->redirect_link, link_len, path_dst);
  server->endpoints[server->endpointsLength - 1] = *path;

  unsigned int src_full_path_len = 0;
  char *src_full_path = printUrlPath(path, &src_full_path_len);
  printf("Creating new redirection: %.*s -> %s\n", (int)src_full_path_len, src_full_path, path_dst);

  free(src_full_path);
}

void freeServer(SERVER* server) {
  closesocket(server->sock);
}