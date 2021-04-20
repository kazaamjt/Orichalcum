#pragma once
#include <filesystem>
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

private:
	Lexer lexer;
	bool debug;

	Token previous;
	Token current;
	Token next;

	void advance();

	std::unique_ptr<ExprAST> parse_expression();
	std::unique_ptr<IntExprAST> parse_int();
	std::unique_ptr<FloatExprAST> parse_float();
	std::unique_ptr<ExprAST> parse_parens();
	std::unique_ptr<VariableExprAST> parse_identifier();
};

} // LibOrichalcum
