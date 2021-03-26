docker create --name orichalcum-build orichalcum-build-linux-x64_86 | Out-Null

docker cp .\include orichalcum-build:/build/include
docker cp .\src orichalcum-build:/build/src
docker cp .\.bazelrc orichalcum-build:/build/.bazelrc
docker cp .\BUILD.bazel orichalcum-build:/build/BUILD.bazel
docker cp .\WORKSPACE orichalcum-build:/build/WORKSPACE

docker start -a orichalcum-build
docker rm orichalcum-build | Out-Null
