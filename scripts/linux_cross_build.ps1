# PRE stuff
$ProjectRoot = "$PSScriptRoot\.."
Push-Location $ProjectRoot
$Hash = (Get-FileHash dockerfile -Algorithm MD5).Hash
$Name = "orichalcum-build-linux-x64_86:$Hash"

(docker image inspect $Name) | Out-Null
if ($LASTEXITCODE -ne 0) {
    docker build -t $Name .
}

docker create -t --name orichalcum-build $Name | Out-Null

docker cp .\include orichalcum-build:/build/include
docker cp .\src orichalcum-build:/build/src
docker cp .\.bazelrc orichalcum-build:/build/.bazelrc
docker cp .\BUILD.bazel orichalcum-build:/build/BUILD.bazel
docker cp .\WORKSPACE orichalcum-build:/build/WORKSPACE

docker start -a orichalcum-build
docker rm orichalcum-build | Out-Null

Pop-Location
