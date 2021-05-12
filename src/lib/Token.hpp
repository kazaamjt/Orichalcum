#pragma once
#include <memory>
#include <string>

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
	COLON,
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
	FALSE_,
	FINALLY,
	FOR,
	FROM,
	GLOBAL,
	IF,
	IMPORT,
	IN_,
	IS_,
	LAMBDA,
	NONE,
	NONLOCAL,
	NOT,
	OR,
	PASS,
	RAISE,
	RETURN,
	STRUCT,
	TRUE_,
	TRY,
	WHILE,
	WITH,
	YIELD,
};

std::string to_string(const TOKEN_TYPE token_type);

struct Index{
	Index(int line, int col);

	int line;
	int col;

	void set(int line, int col);
};

struct Token {
	Token();
	Token(
		const std::string &file_path,
		Index index,
		TOKEN_TYPE type,
		const std::string &content,
		const std::string &line_content
	);

	std::string file_path;
	Index index;
	TOKEN_TYPE type;
	std::string content;
	std::string line_content;
	std::string alt_string();
};

}
