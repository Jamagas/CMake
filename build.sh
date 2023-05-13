#!/bin/bash

echo "The build.sh script is called with the following arguments: $@"

function clean_build() {
  echo "Cleaning build directory..."
  rm -rf build
}

function build() {
  local build_type=$1
  local enable_testing=$2
  echo "Building project with the following settings:"
  echo "Build Type: $build_type"
  echo "Enable Tests: $enable_testing"
  rm -rf build
  mkdir -p build && cd build

  # Regular build without code coverage flags
  cmake .. -DENABLE_TESTING=$enable_testing -DCMAKE_CONFIGURATION_TYPES=$build_type
  cmake --build . --config $build_type
  ctest -VV
  cmake --install . --prefix "bin"
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
  # check if image already exists, if not then build it
  if [[ "$(docker images -q cmake_app 2> /dev/null)" == "" ]]; then
    echo "Building image..."
    docker build -t cmake_app .
  else
    echo "Image already exists"
  fi

  # Run the Docker container
  docker run --rm -it -v $(pwd):/app cmake_app /bin/bash -c "./build.sh $args"
else
  # Build locally
  build $build_type $enable_tests
fi
