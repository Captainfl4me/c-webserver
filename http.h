#ifndef HTTP_H
#define HTTP_H

#include <sys/types.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "header.h"
#include "path_url.h"

enum HTTP_Type { NONE, GET };

typedef struct HTTP_Info {
    enum HTTP_Type type;
    size_t path_size;
    char *path;
} HTTP_Info;

typedef struct HTTP_Request {
    size_t buffer_size;
    char *buffer;

    HTTP_Info info;
    size_t headers_size;
    HTTP_Header* headers;
} HTTP_Request;

typedef struct HTTP_Response {
    int status_code;
    int headers_size;
    HTTP_Header* headers;

    long buffer_size;
    char *buffer;
} HTTP_Response;

HTTP_Request* Http_Request_new(void); 
void parseHTTPRequest(SOCKET* sock, HTTP_Request* request);
HTTP_Response* Http_Response_new(void); 
char* writeResponse(HTTP_Response* response, size_t* buffer_size);

void setHeader(HTTP_Response* request, char* key, char* value);
void parseInfo(int infoLength, char* infoString, HTTP_Info* info);

void freeHTTPRequest(HTTP_Request* request);
void freeHTTPResponse(HTTP_Response* response);

#endif