#include "header.h"

HTTP_Header* createHeader(char* key, char* value){
  HTTP_Header* d = malloc(sizeof(HTTP_Header));
  d->key_size = strlen(key);
  d->key = key;
  d->value_size = strlen(value);
  d->value = value;

  return d;
}

char* writeHeader(HTTP_Header header, size_t* buffer_size) {
  *buffer_size = header.key_size+header.value_size+2;
  char* header_string = (char*) malloc((*buffer_size)*sizeof(char));

  memcpy(header_string, header.key, header.key_size);
  memcpy(header_string + header.key_size, ": ", 2);
  memcpy(header_string + header.key_size + 2, header.value, header.value_size);
  return header_string;
}

void parseHeader(int headerLength, char* headerString, HTTP_Header* header){
  int separatorIndex;
  for(separatorIndex = 0; separatorIndex < headerLength; separatorIndex++) {
    if(headerString[separatorIndex] == ':')
      break;
  }
  header->key_size = separatorIndex;
  header->key = (char*) malloc((header->key_size+1) * sizeof(char));
  header->key[header->key_size] = '\0';
  memcpy(header->key, headerString, separatorIndex);
  
  header->value_size = headerLength-(header->key_size+2);
  header->value = (char*) malloc((header->value_size+1) * sizeof(char));
  header->key[header->value_size] = '\0';
  memcpy(header->value, headerString+header->key_size+2, header->value_size);
  
  //printf("key: %.*s\n", header->key_size, header->key);  
  //printf("value: %.*s\n", header->value_size, header->value);
}