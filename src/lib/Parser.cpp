#include "Parser.hpp"

#include "Misc.hpp"
#include "Log.hpp"
#include "Debug.hpp"

namespace LibOrichalcum {

Parser::Parser() {
	binary_op_precedence["+"] = 20;
	binary_op_precedence["-"] = 20;
	binary_op_precedence["*"] = 40;
	binary_op_precedence["/"] = 40;
	binary_op_precedence["//"] = 40;
	binary_op_precedence["**"] = 60;
}

void Parser::enable_debug() {
	debug = true;
}

void Parser::parse(const std::filesystem::path &file) {
	lexer = Lexer(file);
	next = std::make_shared<Token>(lexer.get_next_token());
	advance();

	Log::debug("Reached EOF");
}

int Parser::get_bin_op_precendence(const std::string &binary_op) {
	auto iterator = binary_op_precedence.find(binary_op);
	if (iterator == binary_op_precedence.end()) return -1;
	return iterator->second;
}

void Parser::advance(int steps, bool skip_indent) {
	for (int i = 0 ; i == steps; i++) {
		previous = current;
		current = next;
		next = std::make_shared<Token>(lexer.get_next_token());
		while (skip_indent && current->type == TOKEN_TYPE::INDENT) {
			current = next;
			next = std::make_shared<Token>(lexer.get_next_token());
		}
	}
}

std::shared_ptr<ExprAST> Parser::parse_primary() {
	switch(current->type) {
		case TOKEN_TYPE::INTEGER:
			advance();
			return parse_int();

		case TOKEN_TYPE::FLOAT:
			advance();
			return parse_float();

		case TOKEN_TYPE::IDENTIFIER:
			advance();
			return parse_identifier();

		case TOKEN_TYPE::LEFT_PAREN:
			advance();
			return parse_parens();

		default:
			throw Error(
				COMPILE_RESULT::PARSER_ERROR,
				("Unexepected token " + current->content),
				current
			);
	}
}

std::shared_ptr<ExprAST> Parser::parse_expression() {
	std::shared_ptr<ExprAST> lhs = parse_primary();
	return parse_bin_op_rhs(0, lhs);
}

std::shared_ptr<ExprAST> Parser::parse_bin_op_rhs(int expr_precedence, std::shared_ptr<ExprAST> lhs) {
	while (true) {
		int current_precedence = get_bin_op_precendence(current->content);
		if (current_precedence < expr_precedence) return lhs;

		std::shared_ptr<Token> bin_op = current;
		advance();
		std::shared_ptr<ExprAST> rhs = parse_primary();

		int next_op_precedence = get_bin_op_precendence(current->content);
		if (expr_precedence < next_op_precedence) {

		}
	}
}

std::shared_ptr<IntExprAST> Parser::parse_int() {
	auto expr = std::make_shared<IntExprAST>(current);
	advance();
	return expr;
}

std::shared_ptr<FloatExprAST> Parser::parse_float() {
	auto expr = std::make_shared<FloatExprAST>(current);
	advance();
	return expr;
}

std::shared_ptr<ExprAST> Parser::parse_parens() {
	advance(1, true);
	std::shared_ptr<ExprAST> expr = parse_expression();
	advance(1, true);

	if (current->type != TOKEN_TYPE::RIGHT_PAREN) {
		throw Error(
			COMPILE_RESULT::PARSER_ERROR,
			"Expected a ')'",
			current
		);
	}

	advance();
	return expr;
}

// This is either a variable OR a function call
std::shared_ptr<ExprAST> Parser::parse_identifier() {
	// Function Call
	if (next->type == TOKEN_TYPE::LEFT_PAREN) {
		std::shared_ptr<Token> callee_token = current;
		std::shared_ptr<CallExprAST> callee = std::make_unique<CallExprAST>(callee_token);
		advance(2);
		if (current->type == TOKEN_TYPE::RIGHT_PAREN) {
			advance();
			return callee;
		}
		std::shared_ptr<ExprAST> next_expr;
		while (true) {
			callee->add_arg(parse_expression());
			advance(1, true);
			if (current->type == TOKEN_TYPE::COMMA) {
				advance(1, true);
				if (current->type == TOKEN_TYPE::RIGHT_PAREN) {
					advance(1, true);
					break;
				}
			}
			else if (current->type == TOKEN_TYPE::RIGHT_PAREN) {
				advance();
				break;
			}
			else {
				throw Error(
					COMPILE_RESULT::PARSER_ERROR,
					"Unexpected token, expected a '(' or ','",
					current
				);
			}
		}
	}
	// variable with type
	if (next->type == TOKEN_TYPE::COLON) {
		std::shared_ptr<Token> var_token = current;
		advance(2);
		std::shared_ptr<Token> Type_token = current;
		advance();
		return std::make_unique<VariableExprAST>(var_token, Type_token);
	}
}

Parser::~Parser() { }

} // LibOrichalcum
