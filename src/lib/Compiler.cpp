#include "Compiler.hpp"

#include <iostream>

#include "Debug.hpp"
#include "Log.hpp"

namespace LibOrichalcum {

CompilerReport::CompilerReport(COMPILE_RESULT _result, std::vector<LogLine> _logs):
result(_result), logs(_logs) { }

CompilerReport::CompilerReport(COMPILE_RESULT _result, const Error &_error, std::vector<LogLine> _logs):
result(_result), error(_error), logs(_logs) { }

Compiler::Compiler(CompilerOptions _options):
options(_options),
parser(_options.debug_parser),
vm(options.debug_vm),
compiling_chunk(std::make_shared<Chunk>(0)) {
	Log::set_level(options.log_level, options.print_output);
	Log::verbose("Log level set to " + to_string(options.log_level));
	main_file = options.file;
	main_module = options.file.parent_path();
}

CompilerReport Compiler::run() {
	try {
		parser.parse(main_file);
		main_loop();
	}
	catch(const Error &error) {
		Log::error(error);
		return CompilerReport(
			error.section,
			error,
			Log::get_logs()
		);
	}
	return CompilerReport(COMPILE_RESULT::SUCCESS, Log::get_logs());
}

std::shared_ptr<Chunk> Compiler::current_chunk() {
	return compiling_chunk;
}

void Compiler::main_loop() {
	while(1) {
		std::shared_ptr<ExprAST> expr = parser.next_expr();
		if (expr->token->type == TOKEN_TYPE::EOF_TOKEN) return;
	}
}

} // namespace Orichalcum
