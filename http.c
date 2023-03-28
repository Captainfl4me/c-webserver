#include "http.h"

#define BUFFER_LENGTH 2048

HTTP_Request* Http_Request_new(void){
  HTTP_Request proto;
  proto.buffer_size = 0;
  proto.buffer = NULL;

  proto.headers_size = 0;
  proto.headers = NULL;

  HTTP_Request* d = malloc(sizeof(HTTP_Request));
  *d = proto;
  return d;
}

HTTP_Response* Http_Response_new(void){
  HTTP_Response* d = malloc(sizeof(HTTP_Request));
  d->buffer_size = 0;
  d->buffer = NULL;

  d->headers_size = 0;
  d->headers = NULL;
  return d;
}

char* writeResponse(HTTP_Response* response, size_t* buffer_size) {
  char* buff;
  size_t buff_size = 0;

  if(response->status_code == 200){
    char firstLine[] = "HTTP/1.1 200 OK\n";
    size_t first_line_size = strlen(firstLine);
    buff = (char*) malloc(first_line_size*sizeof(char));
    buff_size = first_line_size;
    memcpy(buff, firstLine, strlen(firstLine));
  }else if(response->status_code == 404){
    char firstLine[] = "HTTP/1.1 404 Not Found\n";
    size_t first_line_size = strlen(firstLine);
    buff = (char*) malloc(first_line_size*sizeof(char));
    buff_size = first_line_size;
    memcpy(buff, firstLine, strlen(firstLine));
  }else{    
    char firstLine[] = "HTTP/1.1 500 Internal Error\n";
    size_t first_line_size = strlen(firstLine);
    buff = (char*) malloc(first_line_size*sizeof(char));
    buff_size = first_line_size;
    memcpy(buff, firstLine, strlen(firstLine));
  }

  for(int i = 0; i < response->headers_size; i++) {
    size_t new_line_size = 0;
    char* header_line = writeHeader(response->headers[i], &new_line_size);
    
    buff = (char*) realloc(buff, (buff_size + new_line_size + 1)*sizeof(char));
    memcpy(buff + buff_size, header_line, new_line_size);
    buff_size += new_line_size + 1;
    buff[buff_size-1] = '\n';
  }
  buff = (char*) realloc(buff, (buff_size+1)*sizeof(char));
  buff[buff_size++] = '\n';

  if(response->buffer_size > 0) {
    buff = (char*) realloc(buff, (buff_size+response->buffer_size)*sizeof(char));
    memcpy(buff + buff_size, response->buffer, response->buffer_size);
    buff_size += response->buffer_size;
  }

  *buffer_size = buff_size;
  return buff;
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
  info->path = (char*) malloc(path_size * sizeof(char));
  memcpy(info->path, buff, path_size);
}

void setHeader(HTTP_Response* request, char* key, char* value) {
  HTTP_Header header = *createHeader(key, value);

  int indexOfHeader = -1;
  //Check if header already exist
  for(int i = 0; i < request->headers_size; i++) {
    if(strcmp(request->headers[i].key, header.value) == 0){
      request->headers[i] = header;
      indexOfHeader = i;
      return;
    }
  }
  //If not realloc
  request->headers_size++;
  if(request->headers_size <= 1){
    request->headers = (HTTP_Header*) malloc(sizeof(HTTP_Header));
  }else{
    request->headers = (HTTP_Header*) realloc(request->headers, request->headers_size*sizeof(HTTP_Header));
  }
  request->headers[request->headers_size-1] = header;
}

void freeHTTPRequest(HTTP_Request* request) {
  if(request->buffer_size > 0)
      free(request->buffer);

  request->buffer_size = 0;
}
void freeHTTPResponse(HTTP_Response* response) {
  if(response->headers_size > 0)
    free(response->headers);
  response->headers_size = 0;
}
