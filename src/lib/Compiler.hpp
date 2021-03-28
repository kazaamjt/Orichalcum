#pragma once
#include <filesystem>
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "VM.hpp"
#include "Misc.hpp"

namespace OrichalcumLib {

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
	bool debug_vm = false;
	bool debug_lexer = false;
	bool repl = false;
};

class Compiler {
public:
	Compiler(CompilerOptions options);
	CompilerReport run();

private:
	std::unique_ptr<VM> vm = std::make_unique<VM>();

	CompilerReport run_file();
	void repl();

	CompilerOptions options;
	std::filesystem::path current_file;
	std::filesystem::path main_module;
};

} // namespace Orichalcum
