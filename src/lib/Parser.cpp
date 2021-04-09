#include "Parser.hpp"

#include "Misc.hpp"

namespace LibOrichalcum {

Parser::Parser() { }

void Parser::enable_debug() {
	debug = true;
}

void Parser::parse(const std::filesystem::path &file) {
	lexer = Lexer(file);
	_parse();
}

void Parser::_parse() {
	current_token = lexer.get_next_token();
	next_token = lexer.get_next_token();
	while (true) {
		switch (current_token.type) {

			default: {
				throw Misc::Error(
					Misc::COMPILER_SECTION::PARSER,
					"Unexpected token: " + current_token.content,
					current_token.index,
					current_token.file_path
				);
			}
		}
	}
}

void Parser::advance_token() {
	previous_token = current_token;
	current_token = next_token;
	next_token = lexer.get_next_token();
}

Parser::~Parser() { }

} // LibOrichalcum
