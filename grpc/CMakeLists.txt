# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.5)

project(grpc_test)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "-fPIC -O0 -Wall -g")

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


option(DEBUG "debug version for gdb" ON)
if(DEBUG)
    ## 这个宏是 cmake 可以理解的？自动会添加 -g？
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()


include_directories(.)
include_directories(/usr/local/include/)
include_directories(/usr/local/Cellar/grpc/1.24.3/include/)
include_directories(../../3rd/include)

link_directories(../../3rd/lib)
link_directories(/usr/local/Cellar/grpc/1.21.3/lib/)
link_directories(/usr/local/Cellar/protobuf/3.7.1/lib)


FIND_LIBRARY(GFLAGS gflags)
IF(NOT GFLAGS)
    MESSAGE(FATAL_ERROR “gflags not found”)
else()
    MESSAGE("gflags path:  " ${GFLAGS})
ENDIF(NOT GFLAGS)


file(GLOB srcs *.cc proto/*.cc)

## 将所有依赖库放在一起
set(EXTRA_LIBS ${EXTRA_LIBS} protobuf grpc++ grpc)


set(client_src ${srcs})
set(server_src ${srcs})
list(FILTER client_src EXCLUDE REGEX ".*Server.cc")
list(FILTER server_src EXCLUDE REGEX ".*Client.cc")


add_executable(server ${server_src})
target_link_libraries (server ${EXTRA_LIBS})

add_executable(client ${client_src})
target_link_libraries (client ${EXTRA_LIBS})
