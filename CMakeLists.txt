
cmake_minimum_required(VERSION 3.2)
project(c-webserver)

set(SRCS main.c http.c)
set(HEADERS http.h)

link_libraries(ws2_32)

add_executable(server ${SRCS} ${HEADERS})