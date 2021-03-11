#pragma once
#include <memory>
#include <string>

#include "Lexer.hpp"

namespace OrichalcumLib {

class Compiler {
public:
	Compiler(const std::string &file);
	std::unique_ptr<Lexer> lexer;

};

} // namespace Orichalcum
