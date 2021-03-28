#include "Compiler.hpp"

#include <iostream>

#include "Debug.hpp"
#include "Log.hpp"

namespace OrichalcumLib {

Compiler::Compiler(CompilerOptions _options): options(_options) {
	if (options.debug_vm) vm->enable_debug();
	if (options.repl) {
		main_module = std::filesystem::current_path();
	} else {
		current_file = options.file;
		main_module = options.file.parent_path();
	}
}

static COMPILE_RESULT section_to_result(Misc::COMPILER_SECTION section) {
	switch (section) {
		case Misc::COMPILER_SECTION::PARSER: return COMPILE_RESULT::PARSER_ERROR;
		case Misc::COMPILER_SECTION::VM: return COMPILE_RESULT::RUNTIME_ERROR;
	}
}

CompilerReport Compiler::run() {
	try {
		return run_file();
	}
	catch(const Misc::CompileError &error) {
		CompilerReport report;
		report.result = section_to_result(error.section);
		report.error = error;
		return report;
	}
}

CompilerReport Compiler::run_file() {
	CompilerReport report;
	report.result = COMPILE_RESULT::OK;

	if (options.repl) {
		repl();
	} else {
		Lexer lexer(current_file);
		if (options.debug_lexer) lexer.enable_debug();
		Token token = lexer.get_next_token();
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
