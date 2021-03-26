FROM debian:bullseye

RUN apt-get update
RUN apt-get install -y wget gnupg

RUN echo "deb http://apt.llvm.org/bullseye/ llvm-toolchain-bullseye main" >> /etc/apt/sources.list.d/llvm.list
RUN echo "deb-src http://apt.llvm.org/bullseye/ llvm-toolchain-bullseye main" >> /etc/apt/sources.list.d/llvm.list
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -

RUN apt-get update
RUN apt-get install -y clang

WORKDIR /opt/bin

RUN wget https://github.com/bazelbuild/bazel/releases/download/4.0.0/bazel-4.0.0-linux-x86_64
RUN mv bazel-4.0.0-linux-x86_64 bazel
RUN chmod 755 bazel
RUN ./bazel

WORKDIR /build
ENV PATH="/opt/bin:$PATH"
ENV CC="clang"

CMD ["bazel", "build", "main", "--config=clang", "--keep_going"]
