#include "Compiler.hpp"

namespace OrichalcumLib {

Compiler::Compiler(const std::string &file):
	lexer(std::make_unique<Lexer>(file)) { }

} // namespace Orichalcum
