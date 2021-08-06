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

	int get_op_precendence(const std::string &binary_op);
	void set_op_precendence(const std::string &binary_op, int precedence);

	std::shared_ptr<ExprAST> next_expr();

private:
	Lexer lexer;
	bool debug;


	std::map<std::string, int> op_precedence;
	void init_op_precedence();

	std::shared_ptr<Token> previous;
	std::shared_ptr<Token> current;
	std::shared_ptr<Token> next;

	void advance(
		int steps = 1,
		bool skip_indent = false
	);

	std::shared_ptr<ExprAST> parse_expression(bool parens = false);
	std::shared_ptr<ExprAST> parse_primary();
	std::shared_ptr<ExprAST> parse_bin_op_rhs(int op_precedence, std::shared_ptr<ExprAST> lhs, bool parens = false);

	std::shared_ptr<UnaryNegExprAST> parse_unary_neg();
	std::shared_ptr<IntExprAST> parse_int();
	std::shared_ptr<FloatExprAST> parse_float();
	std::shared_ptr<PassExprAST> parse_pass();
	std::shared_ptr<NoneExprAST> parse_none();
	std::shared_ptr<BoolExprAST> parse_bool();
	std::shared_ptr<ExprAST> parse_parens();
	std::shared_ptr<ExprAST> parse_identifier();
	std::shared_ptr<PrototypeAST> parse_prototype();
	std::vector<std::shared_ptr<TopLevelExprAST>> parse_body();
	std::shared_ptr<FunctionAST> parse_function();
	std::shared_ptr<TopLevelExprAST> parse_top_level_expr();

	void print_op_precedence();
	void log_debug(const std::string &line);

	[[ noreturn ]] void syntax_error(const std::string &error);
	[[ noreturn ]] void syntax_error(std::shared_ptr<Token> token, const std::string &error);
};

} // LibOrichalcum
