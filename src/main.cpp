#include <cxxopts.hpp>

#include "lib\Compiler.hpp"

#include "lib\Chunk.hpp"
#include "lib\Debug.hpp"
#include "lib\VM.hpp"

inline OrichalcumLib::CompilerOptions init_options(int argc, char *argv[]) {
	OrichalcumLib::CompilerOptions compiler_options;
	cxxopts::Options options("Orichalcum", "Orichalcum Standard Compiler v0.0.1");
	options.add_options()
		("debug-vm", "Enable debugging")
		("h,help", "Prints this help message")
	;

	auto result = options.parse(argc, argv);
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	}

	if (result.count("debug-vm")) {
		compiler_options.debug_vm = true;
	}

	return compiler_options;
}

int main(int argc, char *argv[]) {
	OrichalcumLib::CompilerOptions compiler_options = init_options(argc, argv);
	OrichalcumLib::VM vm;
	vm.enable_debug();

	OrichalcumLib::Chunk chunk("test");
	chunk.write(static_cast<double>(0.001f), 1);
	chunk.write(static_cast<int64_t>(2021), 2);
	chunk.write(OrichalcumLib::OP_CODE::RETURN, 3);

	vm.interpret(chunk);

	return 0;
}
