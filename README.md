# Orichalcum

Orichalcum is a domain-specific programming language.  
It is meant to be used as an embedable language that helps with large and complicated configuration.  
DevOps tools in particular.  
Looks like Python. Feels like Python. Hopefully.  

The idea is simple:  

`Orichalcum Code -> Compilation -> output model`

This `output model` will describe tasks the host system needs to perform.  
(e.g. create a file with these contents)

Some (potential) use cases:  

- Build automation systems (e.g. jenkins)
- Orchestration tools (e.g. Terraform)
- Build tools (make, bazel is a big inspiration)
- Software that has complex config requirements, like webservers?

## GOALS

- Easy to understand/use:  
Python is very big in DevOps, and I'm very familiar with Python,  
thusly I'll try to make this language look as much like Python,
so it is easy to pick up for anyone familiar with python.  
Potential pitfalls and hard to understand concepts should be avoided.  
Jenkins and it's jenkinsfiles come to mind as an example of what not to do here.  

- Deterministic evaluation  
Executing the same code twice should give the same result. Looking at you Terraform.  

- Hermetic execution  
Execution cannot access the file system, network, or system clock, by default.  
It should be safe to execute untrusted code.  
Ofcourse, what comes out is a set of instructions for the system it is embeded in,
which might access any of these systems and not be safe anymore.  

- Output model  
The `output model` has a set of tasks that each have dependencies.  
This way multiple tasks can be executed in parallel.  
It has to be in a format that is easy to parse by humans and machines alike.  

## Compiler or Interpreter?

Technically, both, I guess?  
Orchalcum compiles code to bytecode for it's own internal execution, and to speed up recompiles.  
Bytecode is then interpreted by an internal `Virtual Machine`.  
External systems should not touch this bytecode.  

The `output model` is returned from executing said bytecode.  
This means the internals look a lot like an interpreter,
while externally it looks a lot like a compiler.  

## Build command

Requires bazel and a suitable C++ compiler to build. (gcc, clang or Visual C++)  

clang-cl on Windows:  
`bazel build main --config=clang-cl --compiler=clang-cl --keep_going`  

cl on Windows:  
`bazel build main --config=cl --keep_going`  

clang on Linux:  
`CC=clang bazel build main --config=clang --keep_going`  

gcc on Linux:  
`bazel build main --config=gcc --keep_going`  

NOTES:

- On Linux gcc 9 or up is required. This is due to the internals using `std::filesystem`  
(Debian 10 is known not to work out of the box, Ubuntu 20.4 and Debian 11 do)  
- While gcc on Linux should compile, the focus is Clang because it emits less false positives
and is available cross-platform. In other words, using GCC might result in a bunch of warnings.  

### Cross build

A docker image with the required tools is provided to perform a Linux build on a Windows host.  
Build the image using `docker build -t orichalcum-build-linux-x64_86 .`  
Then run the provided script (from the project root): `scripts/linux_cross_build.ps1`  

The artifacts resulting from this build are not kept currently.  

### Using docker as a build env

Similarly, the above image can be used as docker build environment by vscode.  
It contains all the tools required.  
Currently, you still have to install the vscode extensions yourself though.  
I recomend the `C/C++` extension by microsoft and the `Bazel` extension by the bazel team.  
All the tools required for both extensions are already provided in the image.  

I currently have not tested this myself though.  

## Resources

Things I read that helped quite a bit:

- [Crafting interpreters](https://craftinginterpreters.com)
- [Llvm compiler frontend tutorial](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend)
- [starlark docs](https://github.com/bazelbuild/starlark)
- "Engineering a Compiler" - Keith Copper and Linda Torczon
- "Compilers - Principles, Techniques, and Tools" (2006 edition) - Alfred V. Aho, Monica S. Lam, Ravi Sethi, Jeffrey D. Ullman
