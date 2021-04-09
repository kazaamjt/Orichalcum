#pragma once
#include <filesystem>
#include <memory>
#include <string>

#include "Lexer.hpp"

namespace LibOrichalcum {

class Parser {
public:
	Parser();
	~Parser();
	void enable_debug();

	void parse(const std::string &content);
	void parse(const std::filesystem::path &file);

private:
	Lexer lexer;
	bool debug;

	Token previous_token;
	Token current_token;
	Token next_token;

	void advance_token();
	void _parse();
};
} // LibOrichalcum
