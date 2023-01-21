#include "http.h"

HTTP_Request* Http_Request_new(void){
    HTTP_Request proto;
    proto.buffer_size = 0;

    HTTP_Request* d = malloc(sizeof(HTTP_Request));
    *d = proto;
    return d;
}

//Return buffer size
void parseHTTPRequest(SOCKET* sock, HTTP_Request* request){
  char buff[256];

  int data_len = recv(*sock, buff, 256, 0);
  while(data_len >= 0){
    //Memory allocation for full buffer
    if(request->buffer_size == 0){
      request->buffer = (char*) malloc(sizeof(char)*data_len);
    }else{
      request->buffer = (char*) realloc(request->buffer, sizeof(char)*(data_len+request->buffer_size));
    }
    //Copy from partial buff to request full buffer
    memcpy(request->buffer+request->buffer_size, buff, data_len);
    request->buffer_size += data_len;

    if(data_len < 256)
      break;
    data_len = recv(*sock, buff, 256, 0);
  }  
}

void setHeaders(HTTP_Request* request) {
  for(int i = 0; i < request->buffer_size; i++) {
    printf("%c", request->buffer[i]);
    if(request->buffer[i] == '\n')
      break;
  }
}

void freeHTTPRequest(HTTP_Request* request) {
    if(request->buffer)
        free(request->buffer);

    request->buffer_size = 0;
}
