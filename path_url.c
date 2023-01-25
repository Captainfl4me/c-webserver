#include "path_url.h"

URL_PATH* parseUrlPath(char* path, int pathLen) {
  URL_PATH* url_path = (URL_PATH*) malloc(sizeof(URL_PATH));
  url_path->segment_number = 0;

  int last_dash_index = 1;
  int i;
  for(i = 1; i < pathLen; i++) { //Start at 1 to discard first /
    if(path[i] == '/') {
      addSegmentToUrlPath(url_path, i-last_dash_index, path+last_dash_index);
      last_dash_index = i;
    }
    if(path[i] == '?')
      break;
  }
  if(i-last_dash_index > 0) {
    addSegmentToUrlPath(url_path, i-last_dash_index, path+last_dash_index);
  }

  return url_path;
}

char* filterUrlPath(char* path, int* pathLen) {
  int i;
  for(i = 1; i < *pathLen; i++) { //Start at 1 to discard first /
    if(path[i] == '?')
      break;
  }
  char *filteredPath = (char*) malloc((i-1)*sizeof(char));
  memcpy(path, path+1, i-1);
  *pathLen = i-1;
  return path;
}

void addSegmentToUrlPath(URL_PATH* url_path, int segment_length, char* segment) {
  url_path->segment_number++;
  if(url_path->segment_number == 1)
    url_path->segments = (URL_SEGMENT*) malloc(sizeof(URL_SEGMENT));
  else
    url_path->segments = (URL_SEGMENT*) realloc(url_path->segments, url_path->segment_number*sizeof(URL_SEGMENT));
    
  url_path->segments[url_path->segment_number-1].segment_length = segment_length;
  url_path->segments[url_path->segment_number-1].segment = (char*) malloc(segment_length*sizeof(char));
  memcpy(url_path->segments[url_path->segment_number-1].segment, segment, segment_length);
}