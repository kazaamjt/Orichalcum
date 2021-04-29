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
	Parser(bool debug);
	~Parser();

	void parse(const std::string &content);
	void parse(const std::filesystem::path &file);

	int get_bin_op_precendence(const std::string &binary_op);
	void set_bin_op_precendence(const std::string &binary_op, int precedence);

private:
	Lexer lexer;
	bool debug;

	void main_loop();

	std::map<std::string, int> binary_op_precedence;
	void init_bin_op_precedence();

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
	std::shared_ptr<PrototypeAST> parse_prototype();
	std::vector<std::shared_ptr<TopLevelExprAST>> parse_body();
	std::shared_ptr<FunctionAST> parse_function();
	std::shared_ptr<TopLevelExprAST> parse_top_level_expr();

	void handle_definition();
	void handle_top_level_expr();

	void print_bin_op_precedence();

	[[ noreturn ]] void syntax_error(const std::string &error);
	[[ noreturn ]] void syntax_error(std::shared_ptr<Token> token, const std::string &error);
};

} // LibOrichalcum
