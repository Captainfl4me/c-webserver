#include "path_url.h"

URL_PATH* parseUrlPathToStruct(char* path, int pathLen) {
  URL_PATH* url_path = (URL_PATH*) malloc(sizeof(URL_PATH));
  url_path->segment_number = 0;

  int last_dash_index = 0;
  int i;
  for(i = 0; i < pathLen; i++) {
    if(path[i] == '/') {
      addSegmentToUrlPath(url_path, i-last_dash_index, path+last_dash_index);
      last_dash_index = i;
    }
    if(path[i] == '?')
      break;
  }
  if(i-(last_dash_index+1) > 0) {
    addSegmentToUrlPath(url_path, i-(last_dash_index+1), path+last_dash_index+1);
  }

  return url_path;
}

bool urlPathCmp(URL_PATH* url_path_dst, URL_PATH* url_path_src) {
  if(url_path_dst->segment_number != url_path_src->segment_number) return false;
  
  for(unsigned int i = 0; i < url_path_dst->segment_number; i++){
    if(url_path_dst->segments[i].segment_length != url_path_src->segments[i].segment_length) return false;
    
    if(memcmp(url_path_dst->segments[i].segment, url_path_src->segments[i].segment, url_path_dst->segments[i].segment_length) != 0) return false;
  }

  return true;
}

void addSegmentToUrlPath(URL_PATH* url_path, int segment_length, char* segment) {
  url_path->segment_number++;
  if(url_path->segment_number == 1)
    url_path->segments = (URL_SEGMENT*) malloc(sizeof(URL_SEGMENT));
  else
    url_path->segments = (URL_SEGMENT*) realloc(url_path->segments, url_path->segment_number*sizeof(URL_SEGMENT));
    
  url_path->segments[url_path->segment_number-1].segment_length = segment_length;
  if(segment_length > 0){
    url_path->segments[url_path->segment_number-1].segment = (char*) malloc(segment_length*sizeof(char));
    memcpy(url_path->segments[url_path->segment_number-1].segment, segment, segment_length);
  }else{
    url_path->segments[url_path->segment_number-1].segment = NULL;
  }
}

char* printUrlPath(URL_PATH* url_path, unsigned int* pathLen) {
  *pathLen = 0;
  char * full_path = NULL;
  unsigned int last_path_len = 0;

  for(unsigned int i = 0; i < url_path->segment_number; i++){
    last_path_len = *pathLen;
    if(*pathLen <= 0){
      *pathLen = url_path->segments[i].segment_length > 0 ? url_path->segments[i].segment_length : 1;;
      full_path = (char*) malloc((*pathLen) * sizeof(char));
    }else{
      *pathLen += url_path->segments[i].segment_length > 0 ? url_path->segments[i].segment_length : 1;
      full_path = (char*) realloc(full_path, (*pathLen) * sizeof(char));
    }
    if(url_path->segments[i].segment_length == 0)
      full_path[last_path_len] = '/';
    else
      memcpy(full_path + last_path_len, url_path->segments[i].segment, url_path->segments[i].segment_length);
  }

  return full_path;
}