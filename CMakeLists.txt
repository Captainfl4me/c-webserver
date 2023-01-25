
cmake_minimum_required(VERSION 3.2)
project(c-webserver)

set(SRCS main.c path_url.c server.c header.c http.c)
set(HEADERS path_url.h server.h header.h http.h)

link_libraries(ws2_32)

add_executable(server ${SRCS} ${HEADERS})