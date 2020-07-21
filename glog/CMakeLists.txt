# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.5)

project(glog_test)


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


include_directories(../../3rd/include)
link_directories(../../3rd/lib)


FIND_LIBRARY(GLOG glog)
IF(NOT GLOG)
    MESSAGE(FATAL_ERROR “glog not found”)
else()
    MESSAGE("gtest path:  " ${GLOG})
ENDIF(NOT GLOG)


# 设置之后，glog的行为会不一样
# add_definitions(-DNDEBUG)

file(GLOB srcs *.cpp)

## 将所有依赖库放在一起
set(EXTRA_LIBS ${EXTRA_LIBS} glog gflags)

add_executable(client ${srcs})
target_link_libraries (client ${EXTRA_LIBS})
#add_dependencies(client ${EXTRA_LIBS})
