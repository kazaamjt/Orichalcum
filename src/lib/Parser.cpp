#include "Parser.hpp"

#include "Misc.hpp"
#include "Log.hpp"
#include "Debug.hpp"

namespace LibOrichalcum {

Parser::Parser() { }

void Parser::enable_debug() {
	debug = true;
}

void Parser::parse(const std::filesystem::path &file) {
	lexer = Lexer(file);
	next = lexer.get_next_token();
	advance();

	Log::debug("Reached EOF");
}

void Parser::advance() {
	previous = current;
	current = next;
	next = lexer.get_next_token();
}

std::unique_ptr<IntExprAST> Parser::parse_int() {
	auto expr = std::make_unique<IntExprAST>(current);
	advance();
	return expr;
}

std::unique_ptr<FloatExprAST> Parser::parse_float() {
	auto expr = std::make_unique<FloatExprAST>(current);
	advance();
	return expr;
}

// std::unique_ptr<ExprAST> Parser::parse_parens() {
// 	advance();
// 	std::unique_ptr<ExprAST> expr = parse_expression();

// 	if (current.type != TOKEN_TYPE::RIGHT_PAREN) {
// 		throw Error(
// 			COMPILE_RESULT::PARSER_ERROR,
// 			"Expected a ')'",
// 			current
// 		);
// 	}

// 	return expr;
// }

// This is either a variable OR a function call
// std::unique_ptr<VariableExprAST> Parser::parse_identifier() {
// 	// FUNCTION CALL
// 	if (next.type == TOKEN_TYPE::LEFT_PAREN) {

// 	}
// }

Parser::~Parser() { }

} // LibOrichalcum
