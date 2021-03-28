#pragma once
#include <filesystem>
#include <string>

namespace OrichalcumLib::Misc {

std::string to_hex(size_t number, int width = 5);

enum class COMPILER_SECTION {
	PARSER,
	VM,
};

class CompileError: public std::runtime_error {
public:
	std::string msg;
	std::filesystem::path file;
	COMPILER_SECTION section;

	CompileError();
	CompileError(const std::string &msg, const std::filesystem::path &file, COMPILER_SECTION section);
};

} // OrichalcumLib
