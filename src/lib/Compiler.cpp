#include "Compiler.hpp"

namespace OrichalcumLib {

void Compiler::compile(const std::string &file) {
	Lexer lexer;
	lexer.load(file);
	lexers.push_back(lexer);
}

} // namespace Orichalcum
