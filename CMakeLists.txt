
cmake_minimum_required(VERSION 3.2)
project(c-webserver)

set(SRCS main.c)
set(HEADERS)

link_libraries(ws2_32)

add_executable(hello ${SRCS} ${HEADERS})