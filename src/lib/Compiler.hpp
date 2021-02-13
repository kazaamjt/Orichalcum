#pragma once
#include <exception>
#include <string>
#include <vector>

#include "Lexer.hpp"

namespace OrichalcumLib {

struct FailedToReadFile: public std::exception {
	std::string file;

	FailedToReadFile(const std::string filename): file(filename) { }
	const char * what() const throw () {
		std::string error = "Failed to open " + file;
		return error.c_str();
	}
};

class Compiler {
public:
	std::string compile(const std::string &file);

	std::vector<Lexer> lexers;

private:
	

};

} // namespace Orichalcum
