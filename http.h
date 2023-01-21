#include <sys/types.h>
#include <winsock2.h>
#include <stdio.h>

typedef struct HTTP_Request {
    int buffer_size;
    char *buffer;
} HTTP_Request;

HTTP_Request* Http_Request_new(void); 
void parseHTTPRequest(SOCKET* sock, HTTP_Request* request);
void setHeaders(HTTP_Request* request);
void freeHTTPRequest(HTTP_Request* request);
