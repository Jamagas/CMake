cmake_minimum_required(VERSION 3.26)

project(Calculator VERSION 1.0)

set(LIBRARY_NAME
	${PROJECT_NAME}
)

set(LIBRARY_HEADERS
	include/Calculator.hpp
)

set(LIBRARY_SOURCES
	src/Calculator.cpp
)

# Create source groups
source_group("Sources" FILES ${LIBRARY_SOURCES})
source_group("Headers" FILES ${LIBRARY_HEADERS})

add_library(${LIBRARY_NAME} STATIC
	${LIBRARY_HEADERS}
	${LIBRARY_SOURCES}
)

# Specify the C++ standard to use
add_library(${LIBRARY_NAME}_compiler_flags INTERFACE)
target_compile_features(${LIBRARY_NAME}_compiler_flags INTERFACE cxx_std_17)

# Set compiler flags for different compilers
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# Set compiler options for the project
target_compile_options(${LIBRARY_NAME}_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-fprofile-arcs;-ftest-coverage;--coverage>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# Set linker options for the project
target_link_options(${LIBRARY_NAME}_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:--coverage>>"
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-fprofile-arcs>>"
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-ftest-coverage>>"
)

target_include_directories(${LIBRARY_NAME} PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

target_include_directories(${LIBRARY_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

# link our compiler flags interface library
target_link_libraries(${LIBRARY_NAME} ${LIBRARY_NAME}_compiler_flags)

# Add the install targets for the project
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "Installation prefix")
install(TARGETS ${LIBRARY_NAME}
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)

install(FILES ${LIBRARY_HEADERS} DESTINATION include)

# Enable testing if the ENABLE_TESTING option is on
if(ENABLE_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif()
