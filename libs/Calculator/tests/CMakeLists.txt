cmake_minimum_required(VERSION 3.26)

project(CalculatorTest)

set(TEST_SOURCES
	main.cpp
	tests.cpp
)

# Create source groups
source_group("Sources" FILES ${TEST_SOURCES})

enable_testing()

# Set compiler flags to enable coverage reports
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

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
	"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-fsanitize=address>>"
)

# Use FetchContent to download and include Google Test
include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	# GIT_TAG        v1.13.0
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

target_link_libraries(${PROJECT_NAME}
	gtest_main 
	${LIBRARY_NAME} # NOTE: This is defined from project above
	${PROJECT_NAME}_compiler_flags
)

add_dependencies(${PROJECT_NAME}
	gtest_main
	gtest
	gmock
	gmock_main
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
# Discover tests
include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME}
	DISCOVERY_MODE PRE_TEST
)
