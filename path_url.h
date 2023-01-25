#ifndef PATH_URL_H
#define PATH_URL_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct URL_SEGMENT {
  unsigned int segment_length;
  char* segment;
} URL_SEGMENT;

typedef struct URL_PATH {
  unsigned int segment_number;
  URL_SEGMENT* segments;
} URL_PATH;

URL_PATH* parseUrlPathToStruct(char* path, int pathLen);
char* filterUrlPath(char* path, int* pathLen);
void addSegmentToUrlPath(URL_PATH* url_path, int segment_length, char* segment);

#endif