
# CMake example

This repository is CMake project structure example originally created for XCode project generation.
But it contains valuable insights how to structure your code that it would be easy to maintain using CMake.

## Table of contents
- [Project structure](#project-structure)
- [Copyright and license](#copyright-and-license)

## Project structure

Project structure is basically one master project with two static libraries.

```
Foo
 ├── CMakeLists.txt
 ├── README.md
 ├── include
 ├── libs
 │   ├── A
 │   │   ├── CMakeLists.txt
 │   │   ├── include
 │   │   │   └── A
 │   │   │       └── A.h
 │   │   ├── src
 │   │   │   └── A.cpp
 │   │   └── test
 │   │       ├── ATests.cpp
 │   │       └── CMakeLists.txt
 │   ├── B
 │   │   ├── CMakeLists.txt
 │   │   ├── include
 │   │   │   └── B
 │   │   │       └── B.h
 │   │   ├── src
 │   │   │   └── B.cpp
 │   │   └── test
 │   │       ├── BTests.cpp
 │   │       └── CMakeLists.txt
 │   └── gtest-1.7.0
 ├── src
 │   └── main.cpp
 └── test
```

## Copyright and license

Code and documentation copyright 2015 Darius Sabaliauskas and released under 
[the MIT license](https://github.com/Jamagas/CMake/blob/master/LICENSE).
