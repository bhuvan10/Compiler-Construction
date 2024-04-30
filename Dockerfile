# Use Ubuntu 22.04.3 as the base image
FROM ubuntu:22.04

# Install GCC, GDB, and Make
RUN apt-get update && \
    apt-get install -y gcc gdb make && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory in the container
WORKDIR /usr/src/app
COPY . .
RUN make
CMD ["./program"]
