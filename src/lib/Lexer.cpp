#include "Lexer.hpp"

#include <exception>
#include <fstream>
#include <iostream>

#include "Debug.hpp"
#include "Log.hpp"

namespace LibOrichalcum {

Lexer::Lexer():
line(-1), col(-1), debug(false) { }

Lexer::Lexer(const std::filesystem::path &file, bool _debug):
line(-1), col(-1), debug(_debug), file_path(std::filesystem::absolute(file).string()) {
	if (file.empty()) {
		std::string err = "No such file " + file_path;
		Log::error(err);
		throw std::runtime_error(err);
	}
	std::ifstream input(file);
	if (!input.is_open()) {
		std::string err = "Failed to open file " + file_path;
		Log::error(err);
		throw std::runtime_error("Failed to open file " + file_path);
	}
	bytes.push_back('\n');
	char byte = 0;
	while (input.get(byte)) {
		bytes.push_back(byte);
	}
	input.close();
	bytes.push_back(EOF);
	next_line();
}

void Lexer::next_line() {
	static size_t i = 0;
	line++;
	current_line = "";
	char next = bytes[i++];
	current_line += next;
	while (next != '\n' && next != EOF) {
		next = bytes[i++];
		current_line += next;
	}
}

char Lexer::next_char() {
	static size_t i = 0;
	static bool _next_line = false;
	if (_next_line) {
		_next_line = false;
		next_line();
		i = 0;
	}
	col = static_cast<int>(i);
	char next = current_line[i++];
	if (next == '\n') {
		_next_line = true;
	}
	if (next == '\0') return next_char();
	return next;
}

Token Lexer::get_next_token() {
	Token token = get_token();
	if (debug) Debug::print_token(token);
	return token;
}

Token Lexer::mk_token(
	const std::string &_file_path,
	Index _index,
	TOKEN_TYPE _type,
	const std::string &_content
	) {
		return Token(
			_file_path,
			_index,
			_type,
			_content,
			// This is to remove the newlines at the end.
			// A better way would probably be to just scan for /n and remove that.
			current_line.substr(0, current_line.size() - 1)
		);
	}

Token Lexer::get_token() {
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
			return mk_token(file_path, index, TOKEN_TYPE::INDENT, indent_token);
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

			return mk_token(file_path, index, TOKEN_TYPE::FLOAT, number);
		}

		return mk_token(file_path, index, TOKEN_TYPE::INTEGER, number);
	}

	// - and return operators
	if (current == '-') {
		current = next_char();
		if (current == '>') {
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::RETURN_TYPE_SIGN, "->");
		}

		return mk_token(file_path, index, TOKEN_TYPE::ARITHMATIC_OP, "-");
	}

	if (current == '*') {
		current = next_char();
		if (current == '*') {
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::ARITHMATIC_OP, "**");
		}

		return mk_token(file_path, index, TOKEN_TYPE::ARITHMATIC_OP, "*");
	}

	if (current == '/') {
		current = next_char();
		if (current == '/') {
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::ARITHMATIC_OP, "//");
		}

		return mk_token(file_path, index, TOKEN_TYPE::ARITHMATIC_OP, "/");
	}

	if (current == '+' || current == '%') {
		std::string op_sign(1, current);
		current = next_char();
		return mk_token(file_path, index, TOKEN_TYPE::ARITHMATIC_OP, op_sign);
	}

	if (current == '=') {
		current = next_char();
		if (current == '=') {
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::COMPARISON_OP, "==");
		}

		return mk_token(file_path, index, TOKEN_TYPE::ASIGNMENT_OP, "=");
	}

	if (current == '!') {
		current = next_char();
		if (current == '=') {
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::COMPARISON_OP, "!=");
		}
		return mk_token(file_path, index, TOKEN_TYPE::NOT, "!");
	}

	if (current == '<') {
		current = next_char();
		if (current == '=') {
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::COMPARISON_OP, "<=");
		}

		return mk_token(file_path, index, TOKEN_TYPE::COMPARISON_OP, "<");
	}

	if (current == '>') {
		current = next_char();
		if (current == '=') {
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::COMPARISON_OP, ">=");
		}

		return mk_token(file_path, index, TOKEN_TYPE::COMPARISON_OP, ">");
	}

	// single char tokens
	switch (current) {
		case '(': {
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::LEFT_PAREN, "(");
		}
		case ')': {
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::RIGHT_PAREN, ")");
		}
		case ',': {
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::COMMA, ",");
		}
		case '.': {
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::DOT, ".");
		}
		case ':': {
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::COLON, ":");
		}

		case '"': {
			std::string string;
			current = next_char();
			while (current != '"') {
				string += current;
				current = next_char();
			}
			current = next_char();
			return mk_token(file_path, index, TOKEN_TYPE::STRING, string);
		}
	}

	if (current == EOF) {
		return mk_token(file_path, index, TOKEN_TYPE::EOF_TOKEN, "EOF");
	}

	std::string unknown(1, current);
	current = next_char();
	return mk_token(file_path, index, TOKEN_TYPE::UNKNOWN, unknown);
}

Token Lexer::scan_identifier(const std::string &identifier, Index index) {
	TOKEN_TYPE token_type;
	if (identifier == "and") token_type = TOKEN_TYPE::AND;
	else if (identifier == "as") token_type = TOKEN_TYPE::AS;
	else if (identifier == "assert") token_type = TOKEN_TYPE::ASSERT;
	else if (identifier == "async") token_type = TOKEN_TYPE::ASYNC;
	else if (identifier == "await") token_type = TOKEN_TYPE::AWAIT;
	else if (identifier == "break") token_type = TOKEN_TYPE::BREAK;
	else if (identifier == "continue") token_type = TOKEN_TYPE::CONTINUE;
	else if (identifier == "constraint") token_type = TOKEN_TYPE::CONSTRAINT;
	else if (identifier == "class") token_type = TOKEN_TYPE::CLASS;
	else if (identifier == "def") token_type = TOKEN_TYPE::DEF;
	else if (identifier == "del") token_type = TOKEN_TYPE::DEL;
	else if (identifier == "elif") token_type = TOKEN_TYPE::ELIF;
	else if (identifier == "else") token_type = TOKEN_TYPE::ELSE;
	else if (identifier == "except") token_type = TOKEN_TYPE::EXCEPT;
	else if (identifier == "False") token_type = TOKEN_TYPE::FALSE;
	else if (identifier == "finally") token_type = TOKEN_TYPE::FINALLY;
	else if (identifier == "for") token_type = TOKEN_TYPE::FOR;
	else if (identifier == "from") token_type = TOKEN_TYPE::FROM;
	else if (identifier == "global") token_type = TOKEN_TYPE::GLOBAL;
	else if (identifier == "if") token_type = TOKEN_TYPE::IF;
	else if (identifier == "import") token_type = TOKEN_TYPE::IMPORT;
	else if (identifier == "in") token_type = TOKEN_TYPE::IN;
	else if (identifier == "is") token_type = TOKEN_TYPE::IS;
	else if (identifier == "lambda") token_type = TOKEN_TYPE::LAMBDA;
	else if (identifier == "None") token_type = TOKEN_TYPE::NONE;
	else if (identifier == "nonlocal") token_type = TOKEN_TYPE::NONLOCAL;
	else if (identifier == "not") token_type = TOKEN_TYPE::NOT;
	else if (identifier == "or") token_type = TOKEN_TYPE::OR;
	else if (identifier == "pass") token_type = TOKEN_TYPE::PASS;
	else if (identifier == "raise") token_type = TOKEN_TYPE::RAISE;
	else if (identifier == "return") token_type = TOKEN_TYPE::RETURN;
	else if (identifier == "struct") token_type = TOKEN_TYPE::STRUCT;
	else if (identifier == "True") token_type = TOKEN_TYPE::TRUE;
	else if (identifier == "try") token_type = TOKEN_TYPE::TRY;
	else if (identifier == "while") token_type = TOKEN_TYPE::WHILE;
	else if (identifier == "with") token_type = TOKEN_TYPE::WITH;
	else if (identifier == "yield") token_type = TOKEN_TYPE::YIELD;
	else token_type = TOKEN_TYPE::IDENTIFIER;

	return mk_token(file_path, index, token_type, identifier);
}

} // namespace LibOrichalcum
