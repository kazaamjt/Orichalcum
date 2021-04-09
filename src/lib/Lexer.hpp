#pragma once
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "Misc.hpp"

namespace LibOrichalcum {

enum class TOKEN_TYPE {
	// Special Tokens
	EMPTY, // used to initialize tokens without data
	EOF_TOKEN,
	UNKNOWN,

	// Gramatical Structures
	INDENT,
	RETURN_TYPE_SIGN,
	IDENTIFIER,
	LEFT_PAREN,
	RIGHT_PAREN,
	COMMA,
	DOT,

	// Operators
	ASIGNMENT_OP,
	ARITHMATIC_OP,
	COMPARISON_OP,
	NOT_OP,

	//Base types
	FLOAT,
	INTEGER,
	STRING,

	// Reserved words
	AND,
	AS,
	ASSERT,
	ASYNC,
	AWAIT,
	BREAK,
	CONSTRAINT,
	CONTINUE,
	CLASS,
	DEF,
	DEL,
	ELIF,
	ELSE,
	EXCEPT,
	FALSE,
	FINALLY,
	FOR,
	FROM,
	GLOBAL,
	IF,
	IMPORT,
	IN,
	IS,
	LAMBDA,
	NONE,
	NONLOCAL,
	NOT,
	OR,
	PASS,
	RAISE,
	RETURN,
	STRUCT,
	TRUE,
	TRY,
	WHILE,
	WITH,
	YIELD,
};

struct Token {
	Token();
	Token(const std::string &file_path, Misc::Index index, TOKEN_TYPE type, const std::string &content);

	std::string file_path;
	Misc::Index index;
	TOKEN_TYPE type;
	std::string content;
};


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

	char next_char();
	Token get_token();
	Token scan_identifier(const std::string &identifier, Misc::Index index);
};

} // namespace LibOrichalcum
