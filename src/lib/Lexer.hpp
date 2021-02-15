#pragma once
#include <string>
#include <vector>

namespace OrichalcumLib {

enum class TokenType {
	EOF_TOKEN,
	INDENT,

	FLOAT,
	INTEGER,

	CLASS,
	DEF,
	RETURN,
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
};

} // namespace OrichalcumLib
