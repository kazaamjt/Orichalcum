#pragma once
#include <filesystem>
#include <stdexcept>
#include <string>


namespace LibOrichalcum::Misc {

std::string to_hex(size_t number, int width = 5);

struct Index{
	Index(int line, int col);

	int line;
	int col;

	void set(int line, int col);
};

enum class COMPILER_SECTION {
	PARSER,
	VM,
};

class Error: public std::runtime_error {
public:
	Error(const COMPILER_SECTION section, const std::string &msg, const Index index, const std::string &file_path);
	Error();

	const COMPILER_SECTION section;
	const std::string msg;
	const Index index;
	const std::string file;
};

} // LibOrichalcum
