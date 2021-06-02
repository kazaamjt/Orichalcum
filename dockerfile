FROM debian:bullseye

ENV env TERM=xterm-256color
RUN apt-get update
RUN apt-get install -y wget gnupg git

RUN echo "deb http://apt.llvm.org/bullseye/ llvm-toolchain-bullseye main" >> /etc/apt/sources.list.d/llvm.list
RUN echo "deb-src http://apt.llvm.org/bullseye/ llvm-toolchain-bullseye main" >> /etc/apt/sources.list.d/llvm.list
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -

RUN apt-get update
RUN apt-get install -y clang

WORKDIR /opt/bin
RUN wget https://github.com/bazelbuild/bazel/releases/download/4.0.0/bazel-4.0.0-linux-x86_64
RUN mv bazel-4.0.0-linux-x86_64 bazel

RUN wget https://github.com/bazelbuild/buildtools/releases/download/4.0.1/buildifier
RUN wget https://github.com/bazelbuild/buildtools/releases/download/4.0.1/buildozer-linux-amd64
RUN wget https://github.com/bazelbuild/buildtools/releases/download/4.0.1/unused_deps-linux-amd64
RUN mv buildozer-linux-amd64 buildozer
RUN mv unused_deps-linux-amd64 unused_deps
RUN chmod 755 bazel buildifier buildozer unused_deps
RUN ./bazel

ENV PATH="/opt/bin:$PATH"
ENV CC="clang"
WORKDIR /build

CMD ["bazel", "run", "tests", "--config=clang", "--keep_going"]
