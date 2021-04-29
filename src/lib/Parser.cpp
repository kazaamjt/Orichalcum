#include "Parser.hpp"

#include "Misc.hpp"
#include "Log.hpp"
#include "Debug.hpp"

namespace LibOrichalcum {

Parser::Parser():
debug(false) {
	init_bin_op_precedence();
}

Parser::Parser(bool _debug):
debug(_debug) {
	init_bin_op_precedence();
}

void Parser::init_bin_op_precedence() {
	binary_op_precedence["+"] = 20;
	binary_op_precedence["-"] = 20;
	binary_op_precedence["*"] = 40;
	binary_op_precedence["/"] = 40;
	binary_op_precedence["//"] = 40;
	binary_op_precedence["**"] = 60;

	Log::debug("Initializing binary operation precedence.");
	print_bin_op_precedence();
}

void Parser::parse(const std::filesystem::path &file) {
	Log::verbose("Parsing file " + file.string());
	lexer = Lexer(file);
	next = std::make_shared<Token>(lexer.get_next_token());
	advance();
}

int Parser::get_bin_op_precendence(const std::string &binary_op) {
	auto iterator = binary_op_precedence.find(binary_op);
	if (iterator == binary_op_precedence.end()) return -1;
	return iterator->second;
}

void Parser::set_bin_op_precendence(const std::string &binary_op, int precedence) {
	Log::debug(
		"Setting operator precedence value for \""
		+ binary_op + "\" to " + std::to_string(precedence)
	);
	binary_op_precedence[binary_op] = precedence;
}

void Parser::advance(int steps, bool skip_indent) {
	for (int i = 0 ; i == steps; i++) {
		if (debug) Log::debug("Parser advance");
		previous = current;
		current = next;
		next = std::make_shared<Token>(lexer.get_next_token());
		while (skip_indent && current->type == TOKEN_TYPE::INDENT) {
			if (debug) Log::debug("Ignoring whitespace token");
			current = next;
			next = std::make_shared<Token>(lexer.get_next_token());
		}
	}
}

std::shared_ptr<ExprAST> Parser::parse_primary() {
	if (current->type == TOKEN_TYPE::INTEGER) return parse_int();
	else if (current->type ==  TOKEN_TYPE::FLOAT) return parse_float();
	else if (current->type == TOKEN_TYPE::IDENTIFIER) return parse_identifier();
	else if (current->type == TOKEN_TYPE::LEFT_PAREN) return parse_parens();
	else {
		throw Error(
			COMPILE_RESULT::PARSER_ERROR,
			("Unexepected token " + current->content),
			current
		);
	}
}

std::shared_ptr<ExprAST> Parser::parse_expression() {
	Log::debug("Parsing expression");
	std::shared_ptr<ExprAST> lhs = parse_primary();
	return parse_bin_op_rhs(0, lhs);
}

// Parse binary operator expressions as one would in math.
std::shared_ptr<ExprAST> Parser::parse_bin_op_rhs(int expr_precedence, std::shared_ptr<ExprAST> lhs) {
	Log::debug("Parsing binary expression.");
	while (true) {
		int current_precedence = get_bin_op_precendence(current->content);
		if (current_precedence < expr_precedence) {
			return lhs;
		}

		std::shared_ptr<Token> bin_op = current;
		advance();
		std::shared_ptr<ExprAST> rhs = parse_primary();

		// if bin_op binds less tightly with rhs than the operator after rhs,
		// let the pending operator take rhs as it's lhs
		int next_op_precedence = get_bin_op_precendence(current->content);
		if (expr_precedence < next_op_precedence) {
			rhs = parse_bin_op_rhs(current_precedence + 1, rhs);
		}

		lhs = std::make_shared<BinaryExprAST>(bin_op, lhs, rhs, debug);
	}
}

std::shared_ptr<IntExprAST> Parser::parse_int() {
	auto expr = std::make_shared<IntExprAST>(current, debug);
	advance();
	return expr;
}

std::shared_ptr<FloatExprAST> Parser::parse_float() {
	auto expr = std::make_shared<FloatExprAST>(current, debug);
	advance();
	return expr;
}

std::shared_ptr<ExprAST> Parser::parse_parens() {
	if (debug) Log::debug("Parsing parentheses expression.");
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
		Log::debug("Identifer followed by parentheses, parsing as function call.");
		std::shared_ptr<Token> callee_token = current;
		std::vector<std::shared_ptr<ExprAST>> args;
		advance(2);
		if (current->type == TOKEN_TYPE::RIGHT_PAREN) {
			advance();
		}
		else {
			std::shared_ptr<ExprAST> next_expr;
			while (true) {
				args.push_back(parse_expression());
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
						"Unexpected token: " + current->alt_string() + " (expected '(' or ',')",
						current
					);
				}
			}
		}
		return std::make_shared<CallExprAST>(callee_token, args, debug);
	}
	// variable with type
	std::shared_ptr<Token> var_token = current;
	advance();
	if (next->type == TOKEN_TYPE::COLON) {
		Log::debug("Parsing variable + type declaration.");
		advance();
		std::shared_ptr<Token> Type_token = current;
		advance();
		return std::make_shared<VariableExprAST>(var_token, Type_token, debug);
	} else {
		Log::debug("Parsing variable.");
		return std::make_shared<VariableExprAST>(var_token, debug);
	}
}

std::shared_ptr<PrototypeAST> Parser::parse_prototype() {
	std::shared_ptr<Token> def_token = current;
	advance();
	if (current->type != TOKEN_TYPE::IDENTIFIER) {
		throw Error(
			COMPILE_RESULT::PARSER_ERROR,
			"Unexpected token: " + current->alt_string() + " (expected a function name.)",
			current
		);
	}

	std::string name = current->content;
	advance();
	if (current->type != TOKEN_TYPE::LEFT_PAREN) {
		throw Error(
			COMPILE_RESULT::PARSER_ERROR,
			"Unexpected token: " + current->alt_string() + " (expected '(').",
			current
		);
	}

	advance(1, true);
	std::vector<std::shared_ptr<FunctionArg>> args;
	while (true) {
		if (current->type == TOKEN_TYPE::IDENTIFIER) {
			advance();
			if (current->type == TOKEN_TYPE::COLON) {
				advance();
				if (next->type == TOKEN_TYPE::IDENTIFIER) {
					args.push_back(
						std::make_shared<FunctionArg>(previous, next, debug)
					);
					advance();
				}
				else {
					throw Error(
						COMPILE_RESULT::PARSER_ERROR,
						"Unexpected token: " + current->alt_string() + " (expected a type declaration.)",
						current
					);
				}
			}
			else {
				throw Error(
					COMPILE_RESULT::PARSER_ERROR,
					"Unexpected token: " + current->alt_string() + " (expected a type declaration.)",
					current
				);
			}
		}
		else if (current->type == TOKEN_TYPE::COMMA) {
			advance(1, true);
		}
		else if (current->type == TOKEN_TYPE::RIGHT_PAREN) {
			advance();
			break;
		}
		else {
			throw Error(
				COMPILE_RESULT::PARSER_ERROR,
				"Unexpected token: " + current->alt_string() + " (expected ',' or ')').",
				current
			);
		}
	}

	if (current->type == TOKEN_TYPE::RETURN_TYPE_SIGN) {
		advance();
		if (current->type == TOKEN_TYPE::IDENTIFIER) {
			advance();
		}
		else {
			throw Error(
				COMPILE_RESULT::PARSER_ERROR,
				"Unexpected token: " + current->alt_string() + " (expected a return type).",
				current
			);
		}
	}
	else {
		throw Error(
			COMPILE_RESULT::PARSER_ERROR,
			"Unexpected token: " + current->alt_string() + " (expected a return type).",
			current
		);
	}

	if (current->type != TOKEN_TYPE::COLON) {
		throw Error(
			COMPILE_RESULT::PARSER_ERROR,
			"Unexpected token: " + current->alt_string() + " (expected a ':').",
			current
		);
	}

	return std::make_shared<PrototypeAST>(name, args, previous, debug);
}

std::shared_ptr<FunctionAST> Parser::parse_function() {
	advance();
	std::shared_ptr<PrototypeAST> proto = parse_prototype();
}

void Parser::print_bin_op_precedence() {
	if (debug) {
		Log::debug("Currently set operator precedence levels:");
		for (auto const& [key, val]: binary_op_precedence) {
			Log::debug(std::to_string(val) + ": " + key);
		}
	}
}

Parser::~Parser() { }

} // LibOrichalcum
