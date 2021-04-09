#pragma once
#include <filesystem>
#include <memory>
#include <string>

#include "Log.hpp"
#include "Misc.hpp"
#include "Parser.hpp"
#include "VM.hpp"

namespace LibOrichalcum {

enum class COMPILE_RESULT {
	SUCCESS,
	PARSER_ERROR,
	RUNTIME_ERROR,
};

struct CompilerReport {
	const COMPILE_RESULT result;
	const Misc::Error error;
	const std::vector<LogLine> logs;

	CompilerReport(COMPILE_RESULT result, std::vector<LogLine> logs);
	CompilerReport(COMPILE_RESULT result, const Misc::Error &error, std::vector<LogLine> logs);
};

struct CompilerOptions {
	std::filesystem::path file;
	LOG_LEVEL log_level = LOG_LEVEL::INFO;
	bool debug_vm = false;
	bool debug_parser = false;
	bool print_output = false;
};

class Compiler {
public:
	Compiler(CompilerOptions options);
	CompilerReport run();

private:
	Parser parser = Parser();
	VM vm = VM();

	void _run();

	CompilerOptions options;
	std::filesystem::path main_file;
	std::filesystem::path main_module;
};

} // namespace Orichalcum
