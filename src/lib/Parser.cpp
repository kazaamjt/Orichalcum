#include "Parser.hpp"

#include "Misc.hpp"
#include "Log.hpp"
#include "Debug.hpp"

namespace LibOrichalcum {

// TODO: new line currently is not correctly parsed as a new instruction

Parser::Parser():
debug(false) {
	init_op_precedence();
}

Parser::Parser(bool _debug):
debug(_debug) {
	init_op_precedence();
}

void Parser::init_op_precedence() {
	op_precedence["="] = 10;
	op_precedence["or"] = 20;
	op_precedence["and"] = 20;
	op_precedence["=="] = 30;
	op_precedence["!="] = 30;
	op_precedence["<"] = 40;
	op_precedence[">"] = 40;
	op_precedence["<="] = 40;
	op_precedence[">="] = 40;
	op_precedence["not"] = 50;
	op_precedence["+"] = 60;
	op_precedence["-"] = 60;
	op_precedence["*"] = 70;
	op_precedence["/"] = 70;
	op_precedence["//"] = 70;
	op_precedence["**"] = 80;
	op_precedence["."] = 90; // call operator

	Log::verbose("Initializing binary operation precedence.");
	print_op_precedence();
}

void Parser::parse(const std::filesystem::path &file) {
	Log::verbose("Parsing file " + file.string());
	lexer = Lexer(file, debug);
	next = std::make_shared<Token>(lexer.get_next_token());
	advance();
}

int Parser::get_op_precendence(const std::string &binary_op) {
	auto iterator = op_precedence.find(binary_op);
	if (iterator == op_precedence.end()) return -1;
	return iterator->second;
}

void Parser::set_op_precendence(const std::string &binary_op, int precedence) {
	Log::debug(
		"Setting operator precedence value for \""
		+ binary_op + "\" to " + std::to_string(precedence)
	);
	op_precedence[binary_op] = precedence;
}

void Parser::advance(int steps, bool skip_indent) {
	log_debug("Parser advance");
	previous = current;
	current = next;
	next = std::make_shared<Token>(lexer.get_next_token());
	while (skip_indent && current->type == TOKEN_TYPE::INDENT) {
		log_debug("Ignoring whitespace token");
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
	else if (current->type == TOKEN_TYPE::PASS) return parse_pass();
	else if (current->type == TOKEN_TYPE::NONE) return parse_none();
	else if (current->type == TOKEN_TYPE::FALSE ||
		current->type == TOKEN_TYPE::TRUE) return parse_bool();
	else if (current->content == "-") return parse_unary_neg();
	else if (current->content == "+") {
		advance();
		return parse_expression();
	}
	else if (current->type == TOKEN_TYPE::INDENT) {
		syntax_error("Unexpected indentation");
	}
	else {
		syntax_error(start_token, "Unexpected token: " + start_token->content);
	}
}

std::shared_ptr<ExprAST> Parser::parse_expression(bool parens) {
	log_debug("Parsing expression");
	std::shared_ptr<ExprAST> lhs = parse_primary();
	return parse_bin_op_rhs(0, lhs, parens);
}

// Parse binary operator expressions as one would in math.
std::shared_ptr<ExprAST> Parser::parse_bin_op_rhs(int expr_precedence, std::shared_ptr<ExprAST> lhs, bool parens) {
	log_debug("Parsing to see if binary expression.");
	while (true) {
		int current_precedence = get_op_precendence(current->content);
		if (current_precedence < expr_precedence) {
			return lhs;
		}
		if (!parens && lhs->token->index.line != current->index.line) {
			return lhs;
		}

		std::shared_ptr<Token> bin_op = current;
		advance();
		std::shared_ptr<ExprAST> rhs = parse_primary();

		// if bin_op binds less tightly with rhs than the operator after rhs,
		// let the pending operator take rhs as it's lhs
		int next_op_precedence = get_op_precendence(current->content);
		if (expr_precedence < next_op_precedence) {
			rhs = parse_bin_op_rhs(current_precedence + 1, rhs);
		}

		OP_CODE op_code;
		if (bin_op->content == "+") op_code = OP_CODE::ADD;
		else if (bin_op->content == "-") op_code = OP_CODE::SUBTRACT;
		else if (bin_op->content == "*") op_code = OP_CODE::MULTIPLY;
		else if (bin_op->content == "/") op_code = OP_CODE::DIVIDE;
		else if (bin_op->content == "**") op_code = OP_CODE::EXPONENTIATION;
		else {
			throw Error(
				COMPILE_RESULT::PARSER_ERROR,
				"Unsupported operation: " + bin_op->content,
				bin_op
			);
		}

		lhs = std::make_shared<BinaryExprAST>(op_code, bin_op, lhs, rhs, debug);
	}
}

std::shared_ptr<UnaryNegExprAST> Parser::parse_unary_neg() {
	std::shared_ptr<Token> neg_token = current;
	advance();
	std::shared_ptr<ExprAST> rhs = parse_primary();
	if (neg_token->index.line != rhs->token->index.line) {
		syntax_error(neg_token, "Expected expression:");
	}
	return std::make_shared<UnaryNegExprAST>(neg_token, rhs, debug);
}

std::shared_ptr<IntExprAST> Parser::parse_int() {
	advance();
	return std::make_shared<IntExprAST>(previous, debug);
}

std::shared_ptr<FloatExprAST> Parser::parse_float() {
	advance();
	return std::make_shared<FloatExprAST>(previous, debug);
}

std::shared_ptr<PassExprAST> Parser::parse_pass() {
	advance();
	return std::make_shared<PassExprAST>(previous, debug);
}

std::shared_ptr<NoneExprAST> Parser::parse_none() {
	advance();
	return std::make_shared<NoneExprAST>(previous, debug);
}

std::shared_ptr<BoolExprAST> Parser::parse_bool() {
	advance();
	return std::make_shared<BoolExprAST>(previous, debug);
}

std::shared_ptr<ExprAST> Parser::parse_parens() {
	log_debug("Parsing parentheses expression.");
	advance(1, true);
	std::shared_ptr<ExprAST> expr = parse_expression(true);

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
		log_debug("Identifer followed by parentheses, parsing as function call.");
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
					syntax_error("Unexpected token: " + current->alt_string() + ". Expected '(' or ','.");
				}
			}
		}
		return std::make_shared<CallExprAST>(callee_token, args, debug);
	}
	// variable with type
	std::shared_ptr<Token> var_token = current;
	advance();
	if (next->type == TOKEN_TYPE::COLON) {
		log_debug("Parsing variable + type declaration.");
		advance();
		std::shared_ptr<Token> Type_token = current;
		advance();
		return std::make_shared<VariableExprAST>(var_token, Type_token, debug);
	} else {
		log_debug("Parsing variable.");
		return std::make_shared<VariableExprAST>(var_token, debug);
	}
}

std::shared_ptr<PrototypeAST> Parser::parse_prototype() {
	std::shared_ptr<Token> def_token = current;
	advance();
	if (current->type != TOKEN_TYPE::IDENTIFIER) {
		syntax_error("Unexpected token: " + current->alt_string() + ". Expected identifier.");
	}

	std::string name = current->content;
	advance();
	if (current->type != TOKEN_TYPE::LEFT_PAREN) {
		syntax_error("Unexpected token: " + current->alt_string() + ". Expected '('.");
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
					syntax_error("Unexpected token: " + current->alt_string() + ". Expected a type declaration.");
				}
			}
			else {
				syntax_error("Unexpected token: " + current->alt_string() + ". Expected a type declaration.");
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
			syntax_error("Unexpected token: " + current->alt_string() + ". Expected ',' or ')'.");
		}
	}

	if (current->type == TOKEN_TYPE::RETURN_TYPE_SIGN) {
		advance();
		if (current->type == TOKEN_TYPE::IDENTIFIER) {
			advance();
		}
		else {
			syntax_error("Unexpected token: " + current->alt_string() + ". Expected a return type.");
		}
	}
	else {
		syntax_error("Unexpected token: " + current->alt_string() + ". Expected a return type.");
	}

	if (current->type != TOKEN_TYPE::COLON) {
		syntax_error("Unexpected token: " + current->alt_string() + ". Expected a ':'.");
	}

	return std::make_shared<PrototypeAST>(name, args, previous, debug);
}

std::vector<std::shared_ptr<TopLevelExprAST>> Parser::parse_body() {
	std::vector<std::shared_ptr<TopLevelExprAST>> body;
	std::string indentation;
	if (current->type == TOKEN_TYPE::INDENT)
		indentation = current->content;
	else {
		syntax_error("Unexpected token: " + current->alt_string() + ". Expected indentation.");
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
	return std::make_shared<TopLevelExprAST>(token, body, debug);
}

std::shared_ptr<ExprAST> Parser::next_expr() {
	if (current->type == TOKEN_TYPE::EOF_TOKEN) {
		Log::debug("Parser reached EOF");
		return std::make_shared<EOFExprAST>(current, debug);
	}
	else if (current->type == TOKEN_TYPE::DEF) return parse_function();
	else return parse_top_level_expr();
}

void Parser::print_op_precedence() {
	if (debug) {
		Log::debug("Currently set operator precedence levels:");
		for (auto const& [key, val]: op_precedence) {
			Log::debug(std::to_string(val) + ": " + key);
		}
	}
}

void Parser::log_debug(const std::string &line) {
	if (debug) Log::debug(line);
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
