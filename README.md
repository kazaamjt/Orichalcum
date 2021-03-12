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
- maybe even software that has complex config requirements, like webservers?  

## GOALS

- Easy to understand/use  

Python is very big in DevOps, this I'll try to make this language look as much like Python,
so it is easy to pick up for anyone familiar with python.  
Potential pitfalls and hard to understand concepts should be avoided.  
Jenkins and it's jenkinsfiles come to mind as an example of what not to do here.  

- Deterministic evaluation  

Executing the same code twice should give the same result. Looking at you Terraform.  

- hermetic execution

Execution cannot access the file system, network, system clock. It is safe to execute untrusted code.  
Ofcourse, what comes out is a set of instructions for the system it is embeded in,
which might access any of these systems.  

- Parallel output model

The `output model` has a set of tasks that each have dependencies.  
This way multiple tasks can be executed in parallel.  

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
`bazel build main --config=clang --compiler=clang --keep_going`

## Resources

Things I read that helped quite a bit:

- [Crafting interpreters](https://craftinginterpreters.com)
- [Llvm compiler frontend tutorial](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend)
- [starlark docs](https://github.com/bazelbuild/starlark)
- "Engineering a Compiler" - Keith Copper and Linda Torczon
- "Compilers - Principles, Techniques, and Tools" (2006 edition) - Alfred V. Aho, Monica S. Lam, Ravi Sethi, Jeffrey D. Ullman
