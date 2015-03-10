
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

add_subdirectory(test/gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(ALibraryUnitTests test/ATests.cpp)
target_link_libraries(ALibraryUnitTests gtest gtest_main ${A_LIBRARY_NAME})
add_test(ALibraryUnitTests ALibraryUnitTests)
