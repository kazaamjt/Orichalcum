#pragma once
#include <exception>
#include <filesystem>
#include <string>
#include <vector>

namespace OrichalcumLib {

enum class TokenType {
	EMPTY, // purely used to initialize tokens without data
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
	Token();
	Token(Index index, TokenType type, const std::string &content);

	Index index;
	TokenType type;
	std::string content;
};


class Lexer {
public:
	Lexer(const std::filesystem::path &file);

	Token get_next_token();

private:
	std::vector<char> bytes;
	int line;
	int col;

	char next_char();
	Token scan_identifier(const std::string &identifier, Index index);
};

class FailedToRead: public std::runtime_error  {
protected:
	std::string msg_;

public:
	std::filesystem::path file;

	FailedToRead(const std::string &msg, const std::filesystem::path &_file):
		std::runtime_error::runtime_error (msg),
		file(_file) { }
};

} // namespace OrichalcumLib
