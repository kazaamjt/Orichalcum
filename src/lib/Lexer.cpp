#include "Lexer.hpp"

#include <fstream>
#include <iostream>

namespace OrichalcumLib {

Token::Token():
	index(Index(0,0)), type(TokenType::EMPTY), content("") { }

Token::Token(Index _index, TokenType _type, const std::string &_content=""):
	index(_index), type(_type), content(_content) { }

Index::Index(int _line, int _col):
	line(_line), col(_col) { }

void Index::set(int _line, int _col) {
	line = _line;
	col = _col;
}

Lexer::Lexer(const std::filesystem::path &file):
	line(0), col(-1)
{
	if (file.empty()) {
		throw FailedToRead("ERROR: No such file" , file);
	}
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

Lexer::Lexer(const std::string &content) {
	for (char c: content) {
		bytes.push_back(c);
	}
	bytes.push_back(EOF);
}

char Lexer::next_char() {
	static size_t i = 0;
	char next = bytes[i];
	col++;
	i++;
	if (next == '\n') {
		line++;
		col = -1;
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
	TokenType token_type;
	if (identifier == "and") token_type = TokenType::AND;
	else if (identifier == "as") token_type = TokenType::AS;
	else if (identifier == "assert") token_type = TokenType::ASSERT;
	else if (identifier == "async") token_type = TokenType::ASYNC;
	else if (identifier == "await") token_type = TokenType::AWAIT;
	else if (identifier == "break") token_type = TokenType::BREAK;
	else if (identifier == "continue") token_type = TokenType::CONTINUE;
	else if (identifier == "class") token_type = TokenType::CLASS;
	else if (identifier == "def") token_type = TokenType::DEF;
	else if (identifier == "del") token_type = TokenType::DEL;
	else if (identifier == "elif") token_type = TokenType::ELIF;
	else if (identifier == "else") token_type = TokenType::ELSE;
	else if (identifier == "except") token_type = TokenType::EXCEPT;
	else if (identifier == "False") token_type = TokenType::FALSE;
	else if (identifier == "finally") token_type = TokenType::FINALLY;
	else if (identifier == "for") token_type = TokenType::FOR;
	else if (identifier == "from") token_type = TokenType::FROM;
	else if (identifier == "global") token_type = TokenType::GLOBAL;
	else if (identifier == "if") token_type = TokenType::IF;
	else if (identifier == "import") token_type = TokenType::IMPORT;
	else if (identifier == "in") token_type = TokenType::IN;
	else if (identifier == "is") token_type = TokenType::IS;
	else if (identifier == "lambda") token_type = TokenType::LAMBDA;
	else if (identifier == "none") token_type = TokenType::NONE;
	else if (identifier == "nonlocal") token_type = TokenType::NONLOCAL;
	else if (identifier == "not") token_type = TokenType::NOT;
	else if (identifier == "or") token_type = TokenType::OR;
	else if (identifier == "pass") token_type = TokenType::PASS;
	else if (identifier == "raise") token_type = TokenType::RAISE;
	else if (identifier == "return") token_type = TokenType::RETURN;
	else if (identifier == "struct") token_type = TokenType::STRUCT;
	else if (identifier == "True") token_type = TokenType::TRUE;
	else if (identifier == "try") token_type = TokenType::TRY;
	else if (identifier == "while") token_type = TokenType::WHILE;
	else if (identifier == "with") token_type = TokenType::WITH;
	else if (identifier == "yield") token_type = TokenType::YIELD;
	else token_type = TokenType::IDENTIFIER;

	return Token(index, token_type, identifier);
}

} // namespace OrichalcumLib
