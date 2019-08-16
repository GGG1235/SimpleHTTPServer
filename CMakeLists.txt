cmake_minimum_required(VERSION 3.14)
project(SimpleHTTPServer)

set(CMAKE_CXX_STANDARD 11)

add_executable(SimpleHTTPServer server.cpp utils/logs utils/output utils/timestamp mthread/semaphore.h init/init.h)