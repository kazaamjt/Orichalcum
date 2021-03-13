#pragma once
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "VM.hpp"

namespace OrichalcumLib {

enum class CompileResult {
	COMPILE_OK,
	LEXER_ERROR,
	PARSER_ERROR,
	RUNTIME_ERROR,
};

struct CompilerReport {
	CompileResult result;
};

struct CompilerOptions {
	bool debug_vm;
};

class Compiler {
public:
	Compiler(const std::string &file);

private:
	std::unique_ptr<Lexer> lexer;
	std::unique_ptr<VM> vm;

};

} // namespace Orichalcum
