#pragma once
#include <filesystem>
#include <memory>
#include <string>

#include "Chunk.hpp"
#include "Log.hpp"
#include "Misc.hpp"
#include "Parser.hpp"
#include "VM.hpp"

namespace LibOrichalcum {

static const std::string VERSION = "0.0.0";

struct CompilerReport {
	const COMPILE_RESULT result;
	const Error error;
	const std::vector<LogLine> logs;

	CompilerReport(COMPILE_RESULT result, std::vector<LogLine> logs);
	CompilerReport(COMPILE_RESULT result, const Error &error, std::vector<LogLine> logs);
};

struct CompilerOptions {
	std::filesystem::path file;
	LOG_LEVEL log_level = LOG_LEVEL::INFO;
	bool debug_vm = false;
	bool debug_parser = false;
	bool print_output = false;
	bool allow_stdout = false;
};

class Compiler {
public:
	Compiler(CompilerOptions options);
	CompilerReport run();

private:
	Parser parser;
	VM vm;
	std::shared_ptr<Chunk> chunk;
	std::shared_ptr<Chunk> current_chunk();

	CompilerOptions options;
	std::filesystem::path main_file;
	std::filesystem::path main_module;
};

} // namespace Orichalcum
