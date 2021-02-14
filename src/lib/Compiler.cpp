#include "Compiler.hpp"

#include <fstream>

namespace OrichalcumLib {

std::string Compiler::compile(const std::string &file) {
	std::ifstream input(file);
	if (!input.is_open()) {
		throw FailedToReadFile(file);
	}

	char byte = 0;
	std::vector<char> bytes;
	while (input.get(byte)) {
		bytes.push_back(byte);
	}
	input.close();

	Lexer lexer;
	lexer.load(bytes);
	lexers.push_back(lexer);

	return "";
}

} // namespace Orichalcum
