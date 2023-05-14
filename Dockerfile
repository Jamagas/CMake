#########################################################
# Build First Stage
#########################################################
FROM ubuntu:latest AS cmake_build

LABEL Name=cmake_build Version=0.0.1

# Update package lists and install required dependencies
RUN apt-get update && \
    apt-get install -y git build-essential libssl-dev python3-pip lcov

# Clone CMake repository and switch to the desired version (3.26 in this case)
RUN git clone https://gitlab.kitware.com/cmake/cmake.git && \
    cd cmake && \
    git checkout v3.26.3 && \
    ./bootstrap && \
    make -j$(nproc) && \
    make install && \
    cd .. && \
    rm -rf cmake

# Install gcovr for code coverage
RUN pip3 install gcovr

# Set PATH to include the installed CMake executable
ENV PATH="/usr/local/bin:${PATH}"

#########################################################
# Build Second Stage
#########################################################
FROM cmake_build AS cmake_app

LABEL Name=cmake_app Version=0.0.1

ARG use_docker=false
ENV USE_DOCKER=$use_docker

WORKDIR /app
VOLUME [ "/app" ]

# Copy the entrypoint script
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

# Set the entrypoint
ENTRYPOINT ["/entrypoint.sh"]
