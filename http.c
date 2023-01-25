#include "http.h"

#define BUFFER_LENGTH 512

HTTP_Request* Http_Request_new(void){
    HTTP_Request proto;
    proto.buffer_size = 0;
    proto.headers_size = 0;

    HTTP_Request* d = malloc(sizeof(HTTP_Request));
    *d = proto;
    return d;
}

//Return buffer size
void parseHTTPRequest(SOCKET* sock, HTTP_Request* request){
  char buff[BUFFER_LENGTH];

  int data_len = recv(*sock, buff, BUFFER_LENGTH, 0);
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

    if(data_len < BUFFER_LENGTH)
      break;
    data_len = recv(*sock, buff, BUFFER_LENGTH, 0);
  }
  
  int buffer_counter = 0;
  bool infoHasBeenRead = 0;
  //parse http headers
  for(int i = 0; i < request->buffer_size; i++) {
    if(request->buffer[i] == '\n'){
      //If instant return here then double \n which means header section is ending
      if(buffer_counter <= 1)
        break;

      if(!infoHasBeenRead) {
        HTTP_Info info;
        parseInfo(buffer_counter, buff, &info);

        request->info = info;
        infoHasBeenRead = 1;
      }else{
        HTTP_Header header;
        parseHeader(buffer_counter, buff, &header);

        //Save header to request
        request->headers_size++;
        if(request->headers_size <= 1){
          request->headers = (HTTP_Header*) malloc(sizeof(HTTP_Header));
        }else{
          request->headers = (HTTP_Header*) realloc(request->headers, request->headers_size*sizeof(HTTP_Header));
        }

        request->headers[request->headers_size-1] = header;
      }
      buffer_counter = 0;
      continue;
    }
    buff[buffer_counter++] = request->buffer[i];
  }
}

void parseInfo(int infoLength, char* infoString, HTTP_Info* info){
  char buff[BUFFER_LENGTH];
  int i;
  for(i = 0; i < infoLength; i++) {
    if(infoString[i] == ' ')
      break;
    buff[i] = infoString[i];
  }
  buff[i] = '\0';
  if(strcmp(buff, "GET") == 0){
    info->type = GET;
  }

  int path_size = 0;
  for(++i; i < infoLength; i++) {
    if(infoString[i] == ' ')
      break;
    buff[path_size++] = infoString[i];
  }
  info->path_size = path_size;
  info->path = (char*) malloc(info->path_size*sizeof(char));
  memcpy(info->path, buff, path_size);
}

void parseHeader(int headerLength, char* headerString, HTTP_Header* header){
  int separatorIndex;
  for(separatorIndex = 0; separatorIndex < headerLength; separatorIndex++) {
    if(headerString[separatorIndex] == ':')
      break;
  }
  header->key_size = separatorIndex;
  header->key = (char*) malloc(header->key_size * sizeof(char));
  memcpy(header->key, headerString, separatorIndex);
  
  header->value_size = headerLength-(header->key_size+2);
  header->value = (char*) malloc(header->value_size * sizeof(char));
  memcpy(header->value, headerString+header->key_size+2, header->value_size);
  
  //printf("key: %.*s\n", header->key_size, header->key);  
  //printf("value: %.*s\n", header->value_size, header->value);
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
