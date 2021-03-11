#pragma once
#include <string>
#include <vector>
#include <exception>

namespace OrichalcumLib {

enum class TokenType {
	EOF_TOKEN,
	INDENT,

	ASIGNMENT_OP,
	ARITHMATIC_OP,
	COMPARISON_OP,
	NOT_OP,
	FLOAT,
	INTEGER,

	AND,
	AS,
	ASSERT,
	ASYNC,
	AWAIT,
	BREAK,
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

	RETURN_SIGN,
	IDENTIFIER,

	UNKNOWN
};


struct Index{
	Index(int line, int col);

	int line;
	int col;

	void set(int line, int col);
};

struct Token {
	Token(Index index, TokenType type, const std::string &content);

	Index index;
	TokenType type;
	std::string content;
};


class Lexer {
public:
	Lexer();
	void load(const std::vector<char> &bytes);
	void load(const std::string &file);

	Token get_next_token();

private:
	std::vector<char> bytes;
	int line;
	int col;

	char next_char();
	Token scan_identifier(const std::string &identifier, Index index);
};

class FailedToRead: public std::exception {
public:
	std::string file;

	FailedToRead(const char *msg, const std::string &_file):
		std::exception(msg),
		file(_file) { }
};

} // namespace OrichalcumLib
