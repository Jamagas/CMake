cmake_minimum_required(VERSION 3.26)

project(MyApp VERSION 1.0)

# Option to enable testing
option(ENABLE_TESTING "Enable Testing" OFF)

# Set some Xcode-specific compiler flags if using GNU or Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_TEST_COVERAGE_FILES "YES")
  set(CMAKE_XCODE_ATTRIBUTE_GCC_INSTRUMENT_PROGRAM_FLOW_ARCS "YES")
endif()

# Specify the C++ standard to use
add_library(${PROJECT_NAME}_compiler_flags INTERFACE)
target_compile_features(${PROJECT_NAME}_compiler_flags INTERFACE cxx_std_17)

# Set compiler flags for different compilers
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# Set compiler options for the project
target_compile_options(${PROJECT_NAME}_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-fprofile-arcs;-ftest-coverage;--coverage>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# Set linker options for the project
target_link_options(${PROJECT_NAME}_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:--coverage>>"
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-fprofile-arcs>>"
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-ftest-coverage>>"
)

# Define CMake variables
option(USE_RINGBUFFER "Enable RingBuffer" ON)
option(USE_CALCULATOR "Enable Calculator" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(AppConfig.hpp.in AppConfig.hpp)

set(APP_SOURCE_DIR
	src
)

set(APP_SOURCE
	${APP_SOURCE_DIR}/main.cpp
)

set(APP_LIBRARIES_DIR
	libs
)

set(APP_LIBRARIES 
	Calculator
	RingBuffer
)

# Create source groups
source_group("Sources" FILES ${APP_SOURCE})
# source_group("Headers" FILES ${header_files})

# Enable testing if the ENABLE_TESTING option is on
if(ENABLE_TESTING)
	enable_testing()
endif()

# Add the executable for the project and link to its libraries and compiler flags
add_executable(${PROJECT_NAME} ${APP_SOURCE})

# add the binary tree to the search path for include files
# so that we will find AppConfig.hpp
target_include_directories(${PROJECT_NAME} PRIVATE
	"${PROJECT_BINARY_DIR}"
)

foreach(LIBRARY ${APP_LIBRARIES})
	add_subdirectory("${APP_LIBRARIES_DIR}/${LIBRARY}")
endforeach(LIBRARY)

target_link_libraries(${PROJECT_NAME} ${APP_LIBRARIES} ${PROJECT_NAME}_compiler_flags)

add_dependencies(${PROJECT_NAME} ${APP_LIBRARIES})

# Add the install targets for the project
install(TARGETS ${PROJECT_NAME} 
	DESTINATION ${CMAKE_BINARY_DIR}/bin
	RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin
)

install(FILES "${PROJECT_BINARY_DIR}/AppConfig.hpp"
	DESTINATION include
)