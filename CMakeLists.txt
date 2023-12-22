cmake_minimum_required(VERSION 3.15.0)
project (async_ds)
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES 
    scsp.cpp
    tests.cpp
    main.cpp
)

add_executable(queue ${SOURCES})