# Use a slim Ubuntu image
FROM ubuntu:24.04-slim

# Install build tools and libraries
RUN apt-get update && apt-get install -y \
    cmake \
    pkg-config \
    libhts-dev \
    build-essential

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Create a build directory and compile using CMake
RUN cmake -S . -B build \
 && cmake --build build 

CMD ["./build/pileup-events"]
