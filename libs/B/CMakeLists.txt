
cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME
    BLibrary
)
set(B_LIBRARY_NAME
    B
)
set(B_LIBRARY_HEADERS_DIR
    include/${B_LIBRARY_NAME}
)
set(B_LIBRARY_HEADERS
    ${B_LIBRARY_HEADERS_DIR}/B.h
)
set(B_LIBRARY_SOURCE_DIR
    src
)
set(B_LIBRARY_SOURCE
    ${B_LIBRARY_SOURCE_DIR}/B.cpp
)

project(${PROJECT_NAME})

add_library(${B_LIBRARY_NAME} STATIC
    ${B_LIBRARY_HEADERS}
    ${B_LIBRARY_SOURCE}
)

target_include_directories(${B_LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${B_LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${B_LIBRARY_NAME}>
)

target_include_directories(${B_LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_subdirectory(test)
