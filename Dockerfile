FROM ubuntu:22.04

ENV DEBIAN_FRONTEND="noninteractive"

RUN apt-get -qqy update && apt-get install -qqy \
    clang-15 clang-format-15 clang-tidy-15 llvm lcov lldb-15 libcunit1 \
    libcunit1-doc libcunit1-dev build-essential python3-pip && \
    apt-get upgrade -qqy && \
    apt-get clean

RUN mkdir build/ && \
    mkdir build/src

RUN python3 -m pip install cpplint matplotlib numpy compiledb

VOLUME ["/build"]

WORKDIR /build

CMD ["make", "-B"]