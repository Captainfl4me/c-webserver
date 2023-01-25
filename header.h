#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <string.h>

typedef struct HTTP_Header {
    size_t key_size;
    char *key;
    size_t value_size;
    char *value;
} HTTP_Header;

HTTP_Header* createHeader(char* key, char* value);
void parseHeader(int headerLength, char* headerString, HTTP_Header* header);
char* writeHeader(HTTP_Header header, size_t* buffer_size);

#endif