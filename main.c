#include <stdio.h>
#include <sys/types.h>
#include <winsock2.h>

#include "server.h"

void indexPage(HTTP_Request *req, HTTP_Response *res);

int main()
{
  WSADATA WSAData;
  WSAStartup(MAKEWORD(2, 0), &WSAData);

  SERVER server;
  configureServer(&server, "127.0.0.1", 4148);

  URL_PATH *url_path = parseUrlPathToStruct("/index", 6);
  createEndpoint(&server, url_path, GET, indexPage);

  URL_PATH *url_path_redirect = parseUrlPathToStruct("/", 1);
  createRedirection(&server, url_path_redirect, "/index");
  
  URL_PATH *url_path_static = parseUrlPathToStruct("/www", 1);
  createStaticEndpoint(&server, url_path_redirect, "www");

  listenningConnection(&server);

  freeServer(&server);
  WSACleanup();
  return 0;
}

void indexPage(HTTP_Request *req, HTTP_Response *res)
{
  setHeader(res, "Content-Type", "text/html; charset=UTF-8");

  res->buffer_size = snprintf(NULL, 0, "Hello world!") + 1;
  if (res->buffer_size > 0)
  {
    if (res->buffer != NULL)
      free(res->buffer);

    res->buffer = (char *)malloc(res->buffer_size * sizeof(char));
    sprintf_s(res->buffer, res->buffer_size, "Hello world!");
  }
}
