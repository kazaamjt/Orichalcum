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
const std::string &_line_content):
	file_path(_file_path),
	index(_index),
	type(_type),
	content(_content),
	line_content(_line_content) { }

std::string to_string(const TOKEN_TYPE token_type) {
	switch(token_type){
		case TOKEN_TYPE::EMPTY: return "EMPTY";
		case TOKEN_TYPE::EOF_TOKEN: return "EOF_TOKEN";
		case TOKEN_TYPE::INDENT: return "INDENT";
		case TOKEN_TYPE::LEFT_PAREN: return "LEFT_PAREN";
		case TOKEN_TYPE::RIGHT_PAREN: return "RIGHT_PAREN";
		case TOKEN_TYPE::COLON: return "COLON";
		case TOKEN_TYPE::COMMA: return "COMMA";
		case TOKEN_TYPE::DOT: return "DOT";
		case TOKEN_TYPE::ASIGNMENT_OP: return "ASIGNMENT_OP";
		case TOKEN_TYPE::ARITHMATIC_OP: return "ARITHMATIC_OP";
		case TOKEN_TYPE::COMPARISON_OP: return "COMPARISON_OP";
		case TOKEN_TYPE::NOT_OP: return "NOT_OP";
		case TOKEN_TYPE::FLOAT: return "FLOAT";
		case TOKEN_TYPE::INTEGER: return "INTEGER";
		case TOKEN_TYPE::AND: return "AND";
		case TOKEN_TYPE::AS: return "AS";
		case TOKEN_TYPE::ASSERT: return "ASSERT";
		case TOKEN_TYPE::ASYNC: return "ASYNC";
		case TOKEN_TYPE::AWAIT: return "AWAIT";
		case TOKEN_TYPE::BREAK: return "BREAK";
		case TOKEN_TYPE::CONSTRAINT: return "CONSTRAINT";
		case TOKEN_TYPE::CONTINUE: return "CONTINUE";
		case TOKEN_TYPE::CLASS: return "CLASS";
		case TOKEN_TYPE::DEF: return "DEF";
		case TOKEN_TYPE::DEL: return "DEL";
		case TOKEN_TYPE::ELIF: return "ELIF";
		case TOKEN_TYPE::ELSE: return "ELSE";
		case TOKEN_TYPE::EXCEPT: return "EXCEPT";
		case TOKEN_TYPE::FALSE: return "FALSE";
		case TOKEN_TYPE::FINALLY: return "FINALLY";
		case TOKEN_TYPE::FOR: return "FOR";
		case TOKEN_TYPE::FROM: return "FROM";
		case TOKEN_TYPE::GLOBAL: return "GLOBAL";
		case TOKEN_TYPE::IF: return "IF";
		case TOKEN_TYPE::IMPORT: return "IMPORT";
		case TOKEN_TYPE::IN: return "IN";
		case TOKEN_TYPE::IS: return "IS";
		case TOKEN_TYPE::LAMBDA: return "LAMBDA";
		case TOKEN_TYPE::NONE: return "NONE";
		case TOKEN_TYPE::NONLOCAL: return "NONLOCAL";
		case TOKEN_TYPE::NOT: return "NOT";
		case TOKEN_TYPE::OR: return "OR";
		case TOKEN_TYPE::PASS: return "PASS";
		case TOKEN_TYPE::RAISE: return "RAISE";
		case TOKEN_TYPE::RETURN: return "RETURN";
		case TOKEN_TYPE::STRUCT: return "STRUCT";
		case TOKEN_TYPE::TRUE: return "TRUE";
		case TOKEN_TYPE::TRY: return "TRY";
		case TOKEN_TYPE::WHILE: return "WHILE";
		case TOKEN_TYPE::WITH: return "WITH";
		case TOKEN_TYPE::YIELD: return "YIELD";
		case TOKEN_TYPE::RETURN_TYPE_SIGN: return "RETURN_TYPE_SIGN";
		case TOKEN_TYPE::IDENTIFIER: return "IDENTIFIER";
		case TOKEN_TYPE::UNKNOWN: return "UNKNOWN";
		case TOKEN_TYPE::STRING: return "STRING";
	}
}

}
