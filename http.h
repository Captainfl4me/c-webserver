#include <sys/types.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdbool.h>

enum HTTP_Type { NONE, GET };

typedef struct HTTP_Info {
    enum HTTP_Type type;
    int path_size;
    char *path;
} HTTP_Info;

typedef struct HTTP_Header {
    int key_size;
    char *key;
    int value_size;
    char *value;
} HTTP_Header;

typedef struct HTTP_Request {
    int buffer_size;
    char *buffer;

    HTTP_Info info;
    int headers_size;
    HTTP_Header* headers;
} HTTP_Request;

HTTP_Request* Http_Request_new(void); 
void parseHTTPRequest(SOCKET* sock, HTTP_Request* request);
void parseHeader(int headerLength, char* headerString, HTTP_Header* header);
void parseInfo(int infoLength, char* infoString, HTTP_Info* info);
void setHeaders(HTTP_Request* request);
void freeHTTPRequest(HTTP_Request* request);
