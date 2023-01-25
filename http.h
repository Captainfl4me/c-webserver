#include <sys/types.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdbool.h>

enum HTTP_Type { NONE, GET };

typedef struct HTTP_Info {
    enum HTTP_Type type;
    size_t path_size;
    char *path;
} HTTP_Info;

typedef struct HTTP_Header {
    size_t key_size;
    char *key;
    size_t value_size;
    char *value;
} HTTP_Header;

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

    int buffer_size;
    char *buffer;
} HTTP_Response;

HTTP_Request* Http_Request_new(void); 
void parseHTTPRequest(SOCKET* sock, HTTP_Request* request);
HTTP_Response* Http_Response_new(void); 
char* writeResponse(HTTP_Response* response, size_t* buffer_size);
void parseHeader(int headerLength, char* headerString, HTTP_Header* header);
void parseInfo(int infoLength, char* infoString, HTTP_Info* info);
HTTP_Header* createHeader(char* key, char* value);
void setHeaders(HTTP_Response* request, HTTP_Header header);
char* writeHeader(HTTP_Header header, size_t* buffer_size);
void freeHTTPRequest(HTTP_Request* request);
void freeHTTPResponse(HTTP_Response* response);
