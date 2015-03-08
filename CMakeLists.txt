
cmake_minimum_required(VERSION 3.0.0)

set(FOO_PROJECT_NAME
    Foo
)
set(FOO_SOURCE_DIR
    src
)
set(FOO_SOURCE
    ${FOO_SOURCE_DIR}/main.cpp
)
set(FOO_LIBRARIES_DIR
    libs
)
set(FOO_LIBRARIES 
    A
    B
)

foreach(LIBRARY ${FOO_LIBRARIES})
    add_subdirectory("${FOO_LIBRARIES_DIR}/${LIBRARY}")
endforeach(LIBRARY)

project(${FOO_PROJECT_NAME})

add_executable(${FOO_PROJECT_NAME} ${FOO_SOURCE})
target_link_libraries(${FOO_PROJECT_NAME} ${FOO_LIBRARIES})
