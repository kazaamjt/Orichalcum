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

void Parser::advance(int steps) {
	for (int i = 0 ; i == steps; i++) {
		previous = current;
		current = next;
		next = lexer.get_next_token();
	}
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

std::unique_ptr<ExprAST> Parser::parse_parens() {
	advance();
	std::unique_ptr<ExprAST> expr = parse_expression();

	if (current.type != TOKEN_TYPE::RIGHT_PAREN) {
		throw Error(
			COMPILE_RESULT::PARSER_ERROR,
			"Expected a ')'",
			current
		);
	}

	return expr;
}

// This is either a variable OR a function call
std::unique_ptr<ExprAST> Parser::parse_identifier() {
	// Function Call
	if (next.type == TOKEN_TYPE::LEFT_PAREN) {
		Token callee_token = current;
		advance(2);
		std::vector<ExprAST> args;
		if (current.type != TOKEN_TYPE::RIGHT_PAREN) {
			while (true) {
				
			}
		}
		return std::make_unique<CallExprAST>(callee_token, args);
	}
	// variable with type
	if (next.type == TOKEN_TYPE::COLON) {
		Token var_token = current;
		advance(2);
		return std::make_unique<VariableExprAST>(var_token, current.content);
	}
}

Parser::~Parser() { }

} // LibOrichalcum
