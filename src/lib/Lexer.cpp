#include "Lexer.hpp"

#include <fstream>
#include <iostream>

namespace OrichalcumLib {

Token::Token(Index _index, TokenType _type, const std::string &_content=""):
	index(_index), type(_type), content(_content) { }

Index::Index(int _line, int _col):
	line(_line + 1), col(_col + 1) { }

void Index::set(int _line, int _col) {
	line = _line + 1;
	col = _col + 1;
}

Lexer::Lexer():
	line(1), col(1) { }

void Lexer::load(const std::vector<char> &_bytes) {
	bytes = _bytes;
}

void Lexer::load(const std::string &file) {
		std::ifstream input(file);
	if (!input.is_open()) {
		std::cout << "ERROR: Failed to read file " << file << std::endl;
	}

	char byte = 0;
	while (input.get(byte)) {
		bytes.push_back(byte);
	}
	input.close();
	bytes.push_back(EOF);
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
	static char current = next_char();
	Index index(line, col);

	if (current == '\n') {
		while (current == '\n') {
			current = next_char();
		}
		std::string indent_token = "";
		index.set(line, col);
		while (isspace(current)) {
			indent_token += current;
			current = next_char();
		}
		if (indent_token.length() > 0) {
			return Token(index, TokenType::INDENT, indent_token);
		}
	}

	while (isspace(current)) {
		current = next_char();
	}
	index.set(line, col);

	if (isalpha(current)) {
		std::string identifier = "";
		while (isalnum(current)) {
			identifier += current;
			current = next_char();
		}

		if (identifier == "class") {
			return Token(index, TokenType::CLASS);
		}
		else if(identifier == "def") {
			return Token(index, TokenType::DEF);
		}
		else if(identifier == "return") {
			return Token(index, TokenType::RETURN);
		}

		return Token(index, TokenType::IDENTIFIER, identifier);
	}

	if (isdigit(current)) {
		std::string number;
		while (isdigit(current)) {
			number += current;
			current = next_char();
		}

		if (current == '.') {
			number += current;
			current = next_char();
			while (isdigit(current)) {
				number += current;
				current = next_char();
			}

			return Token(index, TokenType::FLOAT, number);
		}

		return Token(index, TokenType::INTEGER, number);
	}

	if (current == '-') {
		current = next_char();
		if (current == '>') {
			current = next_char();
			return Token(index, TokenType::RETURN_SIGN, "->");
		}
		return Token(index, TokenType::UNKNOWN, "-");
	}

	if (current == EOF) {
		return Token(index, TokenType::EOF_TOKEN, "EOF");
	}

	std::string unknown(1, current);
	current = next_char();
	return Token(index, TokenType::UNKNOWN, unknown);
}

} // namespace OrichalcumLib
