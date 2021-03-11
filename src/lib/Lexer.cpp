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
		throw FailedToRead("ERROR: Failed to read file" , file);
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

	if (isalpha(current) || current == '_') {
		std::string identifier = "";
		while (isalnum(current) || current == '_') {
			identifier += current;
			current = next_char();
		}

		return scan_identifier(identifier, index);
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

	// - and return operators
	if (current == '-') {
		current = next_char();
		if (current == '>') {
			current = next_char();
			return Token(index, TokenType::RETURN_SIGN, "->");
		}

		return Token(index, TokenType::ARITHMATIC_OP, "-");
	}

	if (current == '*') {
		current = next_char();
		if (current == '*') {
			current = next_char();
			return Token(index, TokenType::ARITHMATIC_OP, "**");
		}

		return Token(index, TokenType::ARITHMATIC_OP, "*");
	}

	if (current == '/') {
		current = next_char();
		if (current == '/') {
			current = next_char();
			return Token(index, TokenType::ARITHMATIC_OP, "//");
		}

		return Token(index, TokenType::ARITHMATIC_OP, "/");
	}

	if (current == '+' || current == '%') {
		std::string op_sign(1, current);
		current = next_char();
		return Token(index, TokenType::ARITHMATIC_OP, op_sign);
	}

	if (current == '=') {
		current = next_char();
		if (current == '=') {
			current = next_char();
			return Token(index, TokenType::COMPARISON_OP, "==");
		}

		return Token(index, TokenType::ASIGNMENT_OP, "=");
	}

	if (current == '!') {
		current = next_char();
		if (current == '=') {
			current = next_char();
			return Token(index, TokenType::COMPARISON_OP, "!=");
		}

		return Token(index, TokenType::ASIGNMENT_OP, "!");
	}

	if (current == '<') {
		current = next_char();
		if (current == '=') {
			current = next_char();
			return Token(index, TokenType::COMPARISON_OP, "<=");
		}

		return Token(index, TokenType::ASIGNMENT_OP, "<");
	}

	if (current == '>') {
		current = next_char();
		if (current == '=') {
			current = next_char();
			return Token(index, TokenType::COMPARISON_OP, ">=");
		}

		return Token(index, TokenType::ASIGNMENT_OP, ">");
	}

	if (current == EOF) {
		return Token(index, TokenType::EOF_TOKEN, "EOF");
	}

	std::string unknown(1, current);
	current = next_char();
	return Token(index, TokenType::UNKNOWN, unknown);
}

Token Lexer::scan_identifier(const std::string &identifier, Index index) {
	if (identifier == "and") return Token(index, TokenType::AND);
	if (identifier == "as") return Token(index, TokenType::AS);
	if (identifier == "assert") return Token(index, TokenType::ASSERT);
	if (identifier == "async") return Token(index, TokenType::ASYNC);
	if (identifier == "await") return Token(index, TokenType::AWAIT);
	if (identifier == "break") return Token(index, TokenType::BREAK);
	if (identifier == "continue") return Token(index, TokenType::CONTINUE);
	if (identifier == "class") return Token(index, TokenType::CLASS);
	if (identifier == "def") return Token(index, TokenType::DEF);
	if (identifier == "del") return Token(index, TokenType::DEL);
	if (identifier == "elif") return Token(index, TokenType::ELIF);
	if (identifier == "else") return Token(index, TokenType::ELSE);
	if (identifier == "except") return Token(index, TokenType::EXCEPT);
	if (identifier == "False") return Token(index, TokenType::FALSE);
	if (identifier == "finally") return Token(index, TokenType::FINALLY);
	if (identifier == "for") return Token(index, TokenType::FOR);
	if (identifier == "from") return Token(index, TokenType::FROM);
	if (identifier == "global") return Token(index, TokenType::GLOBAL);
	if (identifier == "if") return Token(index, TokenType::IF);
	if (identifier == "import") return Token(index, TokenType::IMPORT);
	if (identifier == "in") return Token(index, TokenType::IN);
	if (identifier == "is") return Token(index, TokenType::IS);
	if (identifier == "lambda") return Token(index, TokenType::LAMBDA);
	if (identifier == "none") return Token(index, TokenType::NONE);
	if (identifier == "nonlocal") return Token(index, TokenType::NONLOCAL);
	if (identifier == "not") return Token(index, TokenType::NOT);
	if (identifier == "or") return Token(index, TokenType::OR);
	if (identifier == "pass") return Token(index, TokenType::PASS);
	if (identifier == "raise") return Token(index, TokenType::RAISE);
	if (identifier == "return") return Token(index, TokenType::RETURN);
	if (identifier == "struct") return Token(index, TokenType::STRUCT);
	if (identifier == "True") return Token(index, TokenType::TRUE);
	if (identifier == "try") return Token(index, TokenType::TRY);
	if (identifier == "while") return Token(index, TokenType::WHILE);
	if (identifier == "with") return Token(index, TokenType::WITH);
	if (identifier == "yield") return Token(index, TokenType::YIELD);

	return Token(index, TokenType::IDENTIFIER, identifier);
}

} // namespace OrichalcumLib
