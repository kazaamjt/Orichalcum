#include "Parser.hpp"

#include "Misc.hpp"
#include "Log.hpp"

namespace LibOrichalcum {

Parser::Parser() { }

void Parser::enable_debug() {
	debug = true;
}

void Parser::parse(const std::filesystem::path &file) {
	lexer = Lexer(file);
	next_token = lexer.get_next_token();
	bool parsing = true;
	while (parsing) {
		advance_token();
		consume_token();
		if (next_token.type == TOKEN_TYPE::EOF_TOKEN) {
			parsing = false;
			Log::debug(current_token.file_path + ": Reached EOF");
		}
	}
}

void Parser::consume_token() {
	advance_token();
	switch (current_token.type) {
		case TOKEN_TYPE::UNKNOWN: break;
		// Neither of these should ever happen
		case TOKEN_TYPE::EOF_TOKEN:
		case TOKEN_TYPE::EMPTY:
		default: {
			throw Misc::Error(
				Misc::COMPILER_SECTION::PARSER,
				"Unexpected token: " + current_token.content,
				current_token
			);
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
