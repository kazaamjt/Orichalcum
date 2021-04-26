#pragma once
#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "ExprAST.hpp"
#include "Lexer.hpp"

namespace LibOrichalcum {


class Parser {
public:
	Parser();
	~Parser();
	void enable_debug();

	void parse(const std::string &content);
	void parse(const std::filesystem::path &file);

	std::map<std::string, int> binary_op_precedence;
	int get_bin_op_precendence(const std::string &binary_op);

private:
	Lexer lexer;
	bool debug;

	std::shared_ptr<Token> previous;
	std::shared_ptr<Token> current;
	std::shared_ptr<Token> next;

	void advance(
		int steps = 1,
		bool skip_indent = false
	);

	std::shared_ptr<ExprAST> parse_primary();

	std::shared_ptr<ExprAST> parse_expression();
	std::shared_ptr<ExprAST> parse_bin_op_rhs(int bin_op_precedence, std::shared_ptr<ExprAST> lhs);

	std::shared_ptr<IntExprAST> parse_int();
	std::shared_ptr<FloatExprAST> parse_float();
	std::shared_ptr<ExprAST> parse_parens();
	std::shared_ptr<ExprAST> parse_identifier();
};

} // LibOrichalcum
