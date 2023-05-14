cmake_minimum_required(VERSION 3.0.0)

project(RingBufferTest)

set(TEST_SOURCES
	main.cpp
	tests.cpp
)

# Create source groups
source_group("Sources" FILES ${TEST_SOURCES})

# Set compiler flags to enable coverage reports
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

# Use FetchContent to download and include Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Make Google Test available
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})

# Add include directories for your project and Google Test
target_include_directories(${PROJECT_NAME}
	PRIVATE
	.
)

# Create a compiler flags library to share compile options with the project
add_library(${PROJECT_NAME}_compiler_flags INTERFACE)
target_compile_features(${PROJECT_NAME}_compiler_flags INTERFACE cxx_std_17)

target_compile_options(${PROJECT_NAME}_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;--coverage>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

target_link_options(${PROJECT_NAME}_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:--coverage>>"
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-fprofile-arcs>>"
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-ftest-coverage>>"
)

add_dependencies(${PROJECT_NAME}
	gtest_main
	gtest
	gmock
	gmock_main
)

target_link_libraries(${PROJECT_NAME}
	gtest_main 
	${LIBRARY_NAME} # NOTE: This is defined from project above
	${PROJECT_NAME}_compiler_flags
)

# add_test(${PROJECT_NAME} ${PROJECT_NAME})

# Discover tests
set(CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

if (WIN32)
	message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
	# Add a custom command to copy the required dependencies to the build/Test/Release directory
	add_custom_command(TARGET RingBufferTest POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory
			${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${PROJECT_BINARY_DIR}/../${CMAKE_BUILD_TYPE}/${LIBRARY_NAME}.lib
			${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/gtest.lib
			${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/gtest_main.lib
			${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/gmock.lib
			${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/gmock_main.lib
			${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMENT "Installed libraries to ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
	)
endif()
