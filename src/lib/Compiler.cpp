#include "Compiler.hpp"

#include <iostream>

#include "Debug.hpp"
#include "Log.hpp"

namespace LibOrichalcum {

CompilerReport::CompilerReport(COMPILE_RESULT _result, std::vector<LogLine> _logs):
result(_result), logs(_logs) { }

CompilerReport::CompilerReport(COMPILE_RESULT _result, const Misc::Error &_error, std::vector<LogLine> _logs):
result(_result), error(_error), logs(_logs) { }

Compiler::Compiler(CompilerOptions _options): options(_options) {
	Log::set_level(options.log_level, options.print_output);
	Log::verbose("Log level set to " + to_string(options.log_level));
	if (options.debug_vm) vm.enable_debug();
	if (options.debug_parser) parser.enable_debug();
	main_file = options.file;
	main_module = options.file.parent_path();
}

static COMPILE_RESULT section_to_result(Misc::COMPILER_SECTION section) {
	switch (section) {
		case Misc::COMPILER_SECTION::PARSER: return COMPILE_RESULT::PARSER_ERROR;
		case Misc::COMPILER_SECTION::VM: return COMPILE_RESULT::RUNTIME_ERROR;
	}
}

CompilerReport Compiler::run() {
	try {
		_run();
	}
	catch(const Misc::Error &error) {
		return CompilerReport(
			section_to_result(error.section),
			error,
			Log::get_logs()
		);
	}
	return CompilerReport(COMPILE_RESULT::SUCCESS, Log::get_logs());
}

void Compiler::_run() {
	parser.parse(main_file);
}

} // namespace Orichalcum
