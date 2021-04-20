#pragma once
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "Token.hpp"

namespace LibOrichalcum {

class Lexer {
public:
	Lexer();
	Lexer(const std::filesystem::path &file, bool debug=false);

	Token get_next_token();

private:
	std::vector<char> bytes;
	int line;
	int col;
	bool debug;
	std::string file_path;
	std::string current_line;

	void next_line();
	char next_char();
	Token get_token();
	Token mk_token(const std::string &file_path, Index index, TOKEN_TYPE type, const std::string &content);
	Token scan_identifier(const std::string &identifier, Index index);
};

} // namespace LibOrichalcum
