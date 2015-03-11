
cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME
    BLibraryTests
)
set(B_LIBRARY_TESTS_SOURCE
    BTests.cpp
)

project(${PROJECT_NAME})

enable_testing()

if (NOT (TARGET gtest AND TARGET gtest_main))
    message(FATAL_ERROR "gtest and gtest_main targets NOT found")
endif()

add_executable(${PROJECT_NAME} ${B_LIBRARY_TESTS_SOURCE})
target_link_libraries(${PROJECT_NAME} 
    gtest 
    gtest_main 
    ${B_LIBRARY_NAME}
)
add_test(${PROJECT_NAME} ${PROJECT_NAME})
