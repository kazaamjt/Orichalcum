#include "compiler.hpp"

#include <fstream>

namespace OrichalcumLib {

std::string Compiler::compile(const std::string &file) {
	std::ifstream input(file);
	if (!input.is_open()) {
		
	}
}

} // namespace Orichalcum
