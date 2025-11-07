FROM debian:bookworm-slim

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
  build-essential git cmake pkg-config libhts-dev ca-certificates \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN cmake -S . -B build \
  && cmake --build build 

CMD ["./build/pileup-events"]


# the following would be slimmer, but would need htslib to 
# be statically linked
# # --- build stage ---
# FROM debian:bookworm-slim AS build
# ENV DEBIAN_FRONTEND=noninteractive
# RUN apt-get update && apt-get install -y --no-install-recommends \
#     build-essential git cmake pkg-config libhts-dev ca-certificates \
#   && rm -rf /var/lib/apt/lists/*
# WORKDIR /app
# COPY . .
# RUN cmake -S . -B build \
#   && cmake --build build 

# # --- runtime stage ---
# FROM debian:bookworm-slim
# WORKDIR /app
# COPY --from=build /app/build/pileup-events /app/pileup-events
# CMD ["./app/pileup-events"]
