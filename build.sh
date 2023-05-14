#!/bin/bash

echo "The build.sh script is called with the following arguments: $@"

function clean_build() {
  echo "Cleaning build directory..."
  rm -rf build
}

# Build project for macOS
function build_macos() {
  local build_type=$1
  local enable_testing=$2

  cmake -G Xcode -DCMAKE_BUILD_TYPE=$build_type -DENABLE_TESTING=$enable_testing \
    -DCMAKE_CXX_FLAGS="--coverage -fprofile-arcs -ftest-coverage" \
    -DCMAKE_SHARED_LINKER_FLAGS="--coverage -fstack-protector" \
    -DCMAKE_EXE_LINKER_FLAGS="--coverage -fprofile-arcs -ftest-coverage" ..

  cmake --build . --config $build_type
  cmake --install . --prefix "."
}

# Build project for Linux
function build_linux() {
  local build_type=$1
  local enable_testing=$2

  cmake -DCMAKE_BUILD_TYPE=$build_type -DENABLE_TESTING=$enable_testing \
    -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage --coverage" \
    -DCMAKE_EXE_LINKER_FLAGS="--coverage" ..

  cmake --build . --config $build_type
  cmake --install . --prefix "."
}

# Build project for Windows
function build_windows() {
  local build_type=$1
  local enable_testing=$2

  cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_CONFIGURATION_TYPES=$build_type -DCMAKE_BUILD_TYPE=$build_type -DENABLE_TESTING=$enable_testing ..
  cmake --build . --config $build_type
  cmake --install . --prefix "."
}

# Build project
function build() {
  local build_type=$1
  local enable_testing=$2

  echo "OSTYPE = ${OSTYPE}"  

  echo "Building project with the following settings:"
  echo "Build Type: $build_type"
  echo "Enable Testing: $enable_testing"
  rm -rf build
  mkdir -p build && cd build

  if [[ "$OSTYPE" == "darwin"* ]]; then
    build_macos $build_type $enable_testing
  elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    build_linux $build_type $enable_testing
  elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    build_windows $build_type $enable_testing
  else
    echo "Unsupported operating system"
    sleep 2
  fi

  if [[ "$OSTYPE" != "msys" && "$OSTYPE" != "win32" ]]; then
    if [ "$enable_testing" == "ON" ]; then
      # Run tests and generate coverage report
      echo "Running Tests..."
      rm *.info
      ctest --verbose --output-on-failure
      lcov --directory App --directory Test --directory build --capture --output-file coverage.info
      lcov --remove coverage.info '/usr/include/*' '/Applications/Xcode_13.2.app/*' '/app/build/_deps/*' '/app/build/Testing/*' './build/_deps/*' -o cov_test_filtered.info
      genhtml cov_test_filtered.info --output-directory coverage_report
      echo "Coverage report generated at $(realpath coverage_report/index.html)"
    fi
  fi
}

function build_help() {
  echo "Usage: build.sh [OPTIONS]"
  echo "Builds and runs tests for the ProjName project"
  echo ""
  echo "Options:"
  echo "  --clean        Remove build directory and exit"
  echo "  --debug        Enable DEBUG symbol"
  echo "  --tests        Enable Tests"
  echo "  --all          Build and run tests (default)"
  echo "  --docker       Build using Docker image"
  echo "  -h, --help     Display this help message and exit"
}

# Default options
build_type="Release"
enable_tests="OFF"

args=""
# Parse command line options
while [[ $# -gt 0 ]]; do
  case "$1" in
    --clean)
      clean_build
      exit 0
      ;;
    --debug)
      build_type="Debug"
      args="${args} --debug"
      ;;
    --all)
      build_type="Release"
      enable_tests="ON"
      args="${args} --test"
      ;;
    --test)
      enable_tests="ON"
      args="${args} --test"
      ;;
    -h|--help)
      build_help
      exit 0
      ;;
    --docker)
      use_docker="true"
      ;;
    *)
      echo "Unknown option: $1"
      build_help
      exit 1
      ;;
  esac
  shift
done

# Build inside Docker container
if [ "$use_docker" == "true" ]; then
  # Build inside Docker container
  # docker build --target cmake_base -t cmake_base:latest .
  # docker build --target cmake_base_with_sfml -t cmake_base_with_sfml:latest .
  # docker build -t my_sfml_proj:latest .
  # docker run ---rm it -v $(pwd):/app ProjName-build --all
  docker-compose up && docker-compose down
else
  # Build locally
  build $build_type $enable_tests
fi
