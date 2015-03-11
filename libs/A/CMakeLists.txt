
cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME
    ALibrary
)
set(A_LIBRARY_NAME
    A
)
set(A_LIBRARY_HEADERS_DIR
    include/${A_LIBRARY_NAME}
)
set(A_LIBRARY_HEADERS
    ${A_LIBRARY_HEADERS_DIR}/A.h
)
set(A_LIBRARY_SOURCE_DIR
    src
)
set(A_LIBRARY_SOURCE
    ${A_LIBRARY_SOURCE_DIR}/A.cpp
)

project(${PROJECT_NAME})

add_library(${A_LIBRARY_NAME} STATIC
    ${A_LIBRARY_HEADERS}
    ${A_LIBRARY_SOURCE}
)

target_include_directories(${A_LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${A_LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${A_LIBRARY_NAME}>
)

target_include_directories(${A_LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_subdirectory(test)
