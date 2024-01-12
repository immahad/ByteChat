cmake_minimum_required(VERSION 3.27)
project(IMGrecv C)
link_libraries(ws2_32 wsock32)
set(CMAKE_C_STANDARD 23)

add_executable(IMGrecv main.c)
