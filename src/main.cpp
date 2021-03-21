#include <cxxopts.hpp>

#include "lib\Compiler.hpp"
#include "lib\Log.hpp"

inline OrichalcumLib::CompilerOptions init_options(int argc, char *argv[]) {
	OrichalcumLib::CompilerOptions compiler_options;
	cxxopts::Options options("Orichalcum", "Orichalcum Standard Compiler v0.0.1");
	options
		.positional_help("[Positional args]")
		.show_positional_help();
	options.add_options()
		("f,file", "File to use as input [Positional]", cxxopts::value<std::string>())
		("debug-vm", "Enable debugging")
		("h,help", "Prints this help message")
	;

	options.parse_positional({"file"});
	auto result = options.parse(argc, argv);

	if (result.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	}

	if (result.count("debug-vm")) {
		compiler_options.debug_vm = true;
	}

	if (result.count("file")) {
		compiler_options.file = result["file"].as<std::string>();
	} else {
		std::cout << options.help() << std::endl;
		exit(1);
	}

	return compiler_options;
}

int main(int argc, char *argv[]) {
	OrichalcumLib::CompilerOptions compiler_options = init_options(argc, argv);
	OrichalcumLib::Compiler compiler(compiler_options);

	return 0;
}
