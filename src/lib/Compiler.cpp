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
		return run_unsafe();
	}
	catch(const Misc::CompileError &error) {
		CompilerReport report;
		report.result = section_to_result(error.section);
		report.error = error;
		return report;
	}
}

CompilerReport Compiler::run_unsafe() {
	CompilerReport report;
	report.result = COMPILE_RESULT::OK;

	if (options.repl) {
		repl();
	} else {
		Lexer lexer(options.file);
		Token token = lexer.get_next_token();
		int line = -1;
		while (token.type != TOKEN_TYPE::EOF_TOKEN) {
			if (token.index.line == line) {
				std::cout << "    |";
			} else {
				std::cout << std::right << std::setfill(' ') << std::setw(5) << token.index.line + 1;
				line = token.index.line;
			}
			std::cout << " " << Debug::to_string(token.type) << " " << token.content << std::endl;

			token = lexer.get_next_token();
		}
		std::cout << "    - " << Debug::to_string(token.type) << std::endl;
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
