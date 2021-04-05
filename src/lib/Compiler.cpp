#include "Compiler.hpp"

#include <iostream>

#include "Debug.hpp"
#include "Log.hpp"

namespace LibOrichalcum {

Compiler::Compiler(CompilerOptions _options): options(_options) {
	Log::set_level(options.log_level, options.print_output);
	Log::verbose("Log level set to " + to_string(options.log_level));
	if (options.debug_vm) vm.enable_debug();
	if (options.debug_parser) parser.enable_debug();
	if (options.repl) {
		main_module = std::filesystem::current_path();
	} else {
		main_file = options.file;
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
	CompilerReport report;
	report.result = COMPILE_RESULT::OK;
	try {
		_run();
	}
	catch(const Misc::CompileError &error) {
		report.result = section_to_result(error.section);
		report.error = error;
	}
	return report;
}

void Compiler::_run() {
	if (options.repl) {
		repl();
	} else {
		compile_file(main_file);
	}
}

void Compiler::repl() {
	std::string line;
	while (!std::cin.fail()) {
		std::cout << "> ";
		std::getline(std::cin, line);
		std::cout << line << std::endl;
	}
}

void Compiler::compile_file(const std::filesystem::path &file) {
	parser.parse(file);
}

} // namespace Orichalcum
