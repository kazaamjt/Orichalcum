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
	OK,
	PARSER_ERROR,
	RUNTIME_ERROR,
};

struct CompilerReport {
	COMPILE_RESULT result;
	Misc::CompileError error;
};

struct CompilerOptions {
	std::filesystem::path file;
	LogLevel log_level = Log::get_level();
	bool debug_vm = false;
	bool debug_parser = false;
	bool repl = false;
};

class Compiler {
public:
	Compiler(CompilerOptions options);
	CompilerReport run();

private:
	Parser parser = Parser();
	VM vm = VM();

	void _run();
	void repl();
	void compile_file(const std::filesystem::path &file);

	CompilerOptions options;
	std::filesystem::path main_file;
	std::filesystem::path main_module;
};

} // namespace Orichalcum
