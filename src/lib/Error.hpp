#pragma once
#include <filesystem>
#include <stdexcept>
#include <string>

#include "Token.hpp"

namespace LibOrichalcum {

enum class COMPILE_RESULT {
	SUCCESS,
	PARSER_ERROR,
	RUNTIME_ERROR,
};

enum class COMPILER_SECTION {
	PARSER,
	VM,
};

class Error: public std::runtime_error {
public:
	Error();
	Error(
		const COMPILE_RESULT section,
		const std::string &msg,
		Token token
	);
	Error(
		const COMPILE_RESULT section,
		const std::string &msg,
		const Index index,
		const std::string &file_path,
		const std::string &line_content
	);

	const COMPILE_RESULT section;
	const std::string msg;
	const Index index;
	const std::string file;
	const std::string line_content;
};

} // LibOrichalcum
