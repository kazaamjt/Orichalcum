#include "Compiler.hpp"

#include <iostream>

namespace OrichalcumLib {

Compiler::Compiler(CompilerOptions _options): options(_options) {
	if (options.debug_vm) vm->enable_debug();
	if (options.repl) {
		main_module = std::filesystem::current_path();
	} else {
		size_t i  = 0;
		size_t index_last = 0;
		for (char character: options.file) {
			if (character == '\\') {
				character = '/';
			}
			if (character == '/') {
				index_last = i;
			}
			i++;
		}
		main_module = std::filesystem::current_path().append(options.file.substr(0, index_last));
	}
}

CompilerReport Compiler::run() {
	CompilerReport report;
	report.result = COMPILE_RESULT::OK;

	if (options.repl) {
		repl();
	}

	return report;
}

void Compiler::repl() {
	std::string line;
	while (!std::cin.fail()) {
		std::cout << "> ";
		std::getline(std::cin, line);
		std::cout << line << std::endl;
	}

}

} // namespace Orichalcum
