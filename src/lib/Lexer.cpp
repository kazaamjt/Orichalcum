#include "Lexer.hpp"

#include <iostream>

namespace OrichalcumLib {

Token::Token(Index _index, TokenType _type, const std::string &_content=""):
	index(_index), type(_type), content(_content) { }

Index::Index(int _line, int _col):
	line(_line), col(_col) { }

Lexer::Lexer():
	line(1),
	col(1) { }

void Lexer::load(const std::vector<char> &_bytes) {
	bytes = _bytes;
}

char Lexer::next_char() {
	static size_t i = 0;
	char next = bytes[i];
	col++;
	i++;
	if (next == '\n') {
		line++;
		col = 0;
	}

	return next;
}

Token Lexer::get_next_token() {
	char next = next_char();
	if (next == EOF) {
		return Token(Index(line, col), TokenType::EOF_TOKEN);
	}
	return Token(Index(line, col), TokenType::UNKNOWN, std::string(1, next));
}

} // namespace OrichalcumLib
