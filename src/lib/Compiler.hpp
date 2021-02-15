#pragma once
#include <exception>
#include <string>
#include <vector>

#include "Lexer.hpp"

namespace OrichalcumLib {

class Compiler {
public:
	void compile(const std::string &file);
	// TODO:
	// void compile(const std::vector<char> &bytes);

	std::vector<Lexer> lexers;

private:
	

};

} // namespace Orichalcum
