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

# Create the static library (LIB) and set its name to ${LIBRARY_NAME}.lib
add_library(${LIBRARY_NAME} STATIC
	${LIBRARY_SOURCES}
	${LIBRARY_HEADERS}
)
set_target_properties(${LIBRARY_NAME} PROPERTIES OUTPUT_NAME "${LIBRARY_NAME}" PREFIX "")

# Set compile definitions based on the configuration
target_compile_definitions(${LIBRARY_NAME} PRIVATE
	$<$<CONFIG:Debug>:DEBUG_BUILD>
)

# Set compile definitions based on the configuration
target_compile_definitions(${LIBRARY_NAME} PRIVATE
	$<$<CONFIG:Debug>:DEBUG_BUILD>
)

# Set include directories for linking
target_include_directories(${LIBRARY_NAME} PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

target_include_directories(${LIBRARY_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
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

# Set installable libraries
set(installable_libs ${LIBRARY_NAME} ${LIBRARY_NAME}_compiler_flags)

# Add the install targets for the project
# set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "Installation prefix")

message("---------------------------------------------------------------")
message("${LIBRARY_NAME} build settings:-")
message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message("CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")

# Set install rules
install(TARGETS ${installable_libs} 
	DESTINATION lib
    LIBRARY DESTINATION lib
)
install(CODE "message(\"Installed to lib\")")

install(TARGETS ${installable_libs} 
	RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}
)
install(CODE "message(\"Installed to ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}\")")

install(FILES ${LIBRARY_HEADERS} DESTINATION include)
install(CODE "message(\"Installed headers to include\")")

if (WIN32)
	install(CODE "
	file(GLOB runtime_libs LIST_DIRECTORIES false \"lib/*.lib\")
	foreach(lib IN LISTS runtime_libs)
		message(\"Installing runtime dependency: \${lib}\")
		file(INSTALL \"\${lib}\" DESTINATION \"\${CMAKE_BINARY_DIR}/bin\")
		file(INSTALL \"\${lib}\" DESTINATION \"\${PROJECT_BINARY_DIR}/Test/${CMAKE_BUILD_TYPE}\")
	endforeach()
	")
endif()

# Enable testing if the ENABLE_TESTING option is on
if(ENABLE_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif()
