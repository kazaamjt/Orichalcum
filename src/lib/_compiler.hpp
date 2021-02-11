#pragma once
#include <exception>
#include <string>

namespace OrichalcumLib {

struct FailedToReadFile: public std::exception {
	std::string filename;

	FailedToReadFile(const std::string filename);
	const char * what() const throw () {
		return "Failed to open the file ";
	}
};

class Compiler {
public:
	std::string compile(const std::string &file);
};

} // namespace Orichalcum
