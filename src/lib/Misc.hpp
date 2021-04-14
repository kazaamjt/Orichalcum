#pragma once
#include <filesystem>
#include <stdexcept>
#include <string>

#include "Token.hpp"

namespace LibOrichalcum::Misc {

std::string to_hex(size_t number, int width = 5);

enum class COMPILER_SECTION {
	PARSER,
	VM,
};

class Error: public std::runtime_error {
public:
	Error();
	Error(
		const COMPILER_SECTION section,
		const std::string &msg,
		Token token
	);
	Error(
		const COMPILER_SECTION section,
		const std::string &msg,
		const Index index,
		const std::string &file_path,
		const std::string &line_content
	);

	const COMPILER_SECTION section;
	const std::string msg;
	const Index index;
	const std::string file;
	const std::string line_content;
};

} // LibOrichalcum
