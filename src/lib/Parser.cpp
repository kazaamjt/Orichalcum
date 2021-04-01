#include "Parser.hpp"

namespace LibOrichalcum {

Parser::Parser() {
	
}

void Parser::enable_debug() {
	debug = true;
}

void Parser::parse(const std::string &content) {
	lexer = Lexer(content);
}

void Parser::parse(const std::filesystem::path &file) {
	lexer = Lexer(file);
}

Parser::~Parser() {
	
}

} // LibOrichalcum
