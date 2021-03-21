#pragma once
#include <filesystem>
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "VM.hpp"

namespace OrichalcumLib {

enum class COMPILE_RESULT {
	OK,
	PARSER_ERROR,
	RUNTIME_ERROR,
};

struct CompilerReport {
	COMPILE_RESULT result;
};

struct CompilerOptions {
	std::string file = "";
	bool debug_vm = false;
};

class Compiler {
public:
	Compiler(CompilerOptions options);
	CompilerReport compile();

private:
	std::unique_ptr<Lexer> lexer;
	std::unique_ptr<VM> vm;

	CompilerOptions options;
	std::filesystem::path main_module;
};

} // namespace Orichalcum
