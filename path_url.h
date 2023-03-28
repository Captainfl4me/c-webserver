#ifndef PATH_URL_H
#define PATH_URL_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct HTTP_Response HTTP_Response;
typedef struct HTTP_Request HTTP_Request;

enum URL_Type { NOACTION, ENDPOINT, REDIRECTION };

typedef struct URL_SEGMENT {
  unsigned int segment_length;
  char* segment;
} URL_SEGMENT;

typedef struct URL_PATH {
  unsigned int segment_number;
  URL_SEGMENT* segments;
  void (*callback)(HTTP_Request*, HTTP_Response*);
  char *redirect_link;

  enum URL_type type;
} URL_PATH;

URL_PATH* parseUrlPathToStruct(char* path, int pathLen);
void addSegmentToUrlPath(URL_PATH* url_path, int segment_length, char* segment);
bool urlPathCmp(URL_PATH* url_path_dst, URL_PATH* url_path_src);
char* printUrlPath(URL_PATH* url_path, unsigned int* pathLen);

#endif