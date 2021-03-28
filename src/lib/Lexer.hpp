#pragma once
#include <exception>
#include <filesystem>
#include <string>
#include <vector>

namespace OrichalcumLib {

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


struct Index{
	Index(int line, int col);

	int line;
	int col;

	void set(int line, int col);
};

struct Token {
	Token();
	Token(Index index, TOKEN_TYPE type, const std::string &content);

	Index index;
	TOKEN_TYPE type;
	std::string content;
};


class Lexer {
public:
	Lexer(const std::filesystem::path &file);
	Lexer(const std::string &content);

	Token get_next_token();
	void enable_debug();

private:
	std::vector<char> bytes;
	int line;
	int col;
	bool debug = false;

	char next_char();
	Token get_token();
	Token scan_identifier(const std::string &identifier, Index index);
};

} // namespace OrichalcumLib
