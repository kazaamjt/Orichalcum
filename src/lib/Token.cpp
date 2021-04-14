#include "Token.hpp"

namespace LibOrichalcum {

Index::Index(int _line, int _col):
	line(_line), col(_col) { }

void Index::set(int _line, int _col) {
	line = _line;
	col = _col;
}

Token::Token():
	index(Index(0,0)), type(TOKEN_TYPE::EMPTY), content("") { }

Token::Token(
	const std::string &_file_path,
	Index _index,
	TOKEN_TYPE _type,
	const std::string &_content,
	const std::string &_line_content
	):
	file_path(_file_path),
	index(_index),
	type(_type),
	content(_content),
	line_content(_line_content) { }

}
