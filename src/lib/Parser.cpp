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
	lexer = Lexer(file, debug);
	next = std::make_shared<Token>(lexer.get_next_token());
	advance();
	main_loop();
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
	if (debug) Log::debug("Parser advance");
	previous = current;
	current = next;
	next = std::make_shared<Token>(lexer.get_next_token());
	while (skip_indent && current->type == TOKEN_TYPE::INDENT) {
		if (debug) Log::debug("Ignoring whitespace token");
		current = next;
		next = std::make_shared<Token>(lexer.get_next_token());
	}
	steps--;
	if (steps > 0) {
		advance(steps, skip_indent);
	}
}

std::shared_ptr<ExprAST> Parser::parse_primary() {
	std::shared_ptr<Token> start_token = current;
	if (current->type == TOKEN_TYPE::INTEGER) return parse_int();
	else if (current->type ==  TOKEN_TYPE::FLOAT) return parse_float();
	else if (current->type == TOKEN_TYPE::IDENTIFIER) return parse_identifier();
	else if (current->type == TOKEN_TYPE::LEFT_PAREN) return parse_parens();
	else if (current->type == TOKEN_TYPE::PASS) return std::make_shared<PassExprAST>(current, debug);
	else if (current->type == TOKEN_TYPE::INDENT) {
		syntax_error("Unexpected indentation");
	}
	else {
		syntax_error(start_token, "Unexpected token " + current->content);
	}
}

std::shared_ptr<ExprAST> Parser::parse_expression() {
	Log::debug("Parsing expression");
	std::shared_ptr<ExprAST> lhs = parse_primary();
	return parse_bin_op_rhs(0, lhs);
}

// Parse binary operator expressions as one would in math.
std::shared_ptr<ExprAST> Parser::parse_bin_op_rhs(int expr_precedence, std::shared_ptr<ExprAST> lhs) {
	Log::debug("Parsing to see if binary expression.");
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

	if (current->type != TOKEN_TYPE::RIGHT_PAREN) {
		syntax_error("Expected a ')'");
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
			while (true) {
				args.push_back(parse_expression());
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
					syntax_error("Unexpected token: " + current->alt_string() + " (expected '(' or ',')");
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
		syntax_error("Unexpected token: " + current->alt_string() + " (expected a function name.)");
	}

	std::string name = current->content;
	advance();
	if (current->type != TOKEN_TYPE::LEFT_PAREN) {
		syntax_error("Unexpected token: " + current->alt_string() + " (expected '(').");
	}

	advance(1, true);
	std::vector<std::shared_ptr<FunctionArg>> args;
	while (true) {
		if (current->type == TOKEN_TYPE::IDENTIFIER) {
			advance();
			if (current->type == TOKEN_TYPE::COLON) {
				if (next->type == TOKEN_TYPE::IDENTIFIER) {
					args.push_back(
						std::make_shared<FunctionArg>(previous, next, debug)
					);
					advance(2);
				}
				else {
					syntax_error("Unexpected token: " + current->alt_string() + " (expected a type declaration.)");
				}
			}
			else {
				syntax_error("Unexpected token: " + current->alt_string() + " (expected a type declaration.)");
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
			syntax_error("Unexpected token: " + current->alt_string() + " (expected ',' or ')').");
		}
	}

	if (current->type == TOKEN_TYPE::RETURN_TYPE_SIGN) {
		advance();
		if (current->type == TOKEN_TYPE::IDENTIFIER) {
			advance();
		}
		else {
			syntax_error("Unexpected token: " + current->alt_string() + " (expected a return type).");
		}
	}
	else {
		syntax_error("Unexpected token: " + current->alt_string() + " (expected a return type).");
	}

	if (current->type != TOKEN_TYPE::COLON) {
		syntax_error("Unexpected token: " + current->alt_string() + " (expected a ':').");
	}

	return std::make_shared<PrototypeAST>(name, args, previous, debug);
}

std::vector<std::shared_ptr<TopLevelExprAST>> Parser::parse_body() {
	std::vector<std::shared_ptr<TopLevelExprAST>> body;
	std::string indentation;
	if (current->type == TOKEN_TYPE::INDENT)
		indentation = current->content;
	else {
		syntax_error("Unexpected token: " + current->alt_string() + " (expected indentation).");
	}

	while(1) {
		if (current->content == indentation) {
				advance();
				body.push_back(parse_top_level_expr());
		}
		else break;
	}

	return body;
}

std::shared_ptr<FunctionAST> Parser::parse_function() {
	std::shared_ptr<Token> def_token = current;
	std::shared_ptr<PrototypeAST> proto = parse_prototype();
	advance();
	std::vector<std::shared_ptr<TopLevelExprAST>> body = parse_body();
	return std::make_shared<FunctionAST>(def_token, proto, body, debug);
}

std::shared_ptr<TopLevelExprAST> Parser::parse_top_level_expr() {
	if (
		current->type == TOKEN_TYPE::INDENT &&
		current->index.line == next->index.line
	) {
		current->index.col = next->index.col;
		syntax_error(current, "Bad indentation:");
	}
	std::shared_ptr<Token> token = current;
	std::shared_ptr<ExprAST> body = parse_expression();
	advance();
	return std::make_shared<TopLevelExprAST>(token, body, debug);
}

void Parser::handle_definition() {
	std::shared_ptr<FunctionAST> function_ast = parse_function();
}

void Parser::handle_top_level_expr() {
	std::shared_ptr<TopLevelExprAST> top_level_ast = parse_top_level_expr();
}

void Parser::main_loop() {
	while(1) {
		if (current->type == TOKEN_TYPE::EOF_TOKEN) {
			Log::debug("Parser reached EOF");
			return;
		}
		else if (current->type == TOKEN_TYPE::DEF) handle_definition();
		else handle_top_level_expr();
	}
}

void Parser::print_bin_op_precedence() {
	if (debug) {
		Log::debug("Currently set operator precedence levels:");
		for (auto const& [key, val]: binary_op_precedence) {
			Log::debug(std::to_string(val) + ": " + key);
		}
	}
}

void Parser::syntax_error(const std::string &error) {
	syntax_error(current, error);
}

void Parser::syntax_error(std::shared_ptr<Token> token, const std::string &error) {
	throw Error(
		COMPILE_RESULT::PARSER_ERROR,
		error,
		token
	);
}

Parser::~Parser() { }

} // LibOrichalcum
