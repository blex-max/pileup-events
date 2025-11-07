FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    cmake \
    pkg-config \
    libhts-dev

WORKDIR /app

COPY . .

RUN cmake -S . -B build \
 && cmake --build build 

CMD ["./build/pileup-events"]
