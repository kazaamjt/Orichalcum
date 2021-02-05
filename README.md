# Orichalcum

## Build command

Requires bazel and a suitable C++ compiler to build. (gcc, clang or Visual C++)  

clang-cl on Windows:
`bazel build main --config=clang-cl --compiler=clang-cl --keep_going`

cl on Windows:
`bazel build main --config=cl --keep_going`

clang on Linux:
`bazel build main --config=clang --compiler=clang --keep_going`
