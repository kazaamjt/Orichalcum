#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "Lexer.hpp"

namespace LibOrichalcum {

struct ExprAST {
	std::shared_ptr<Token> token;
	ExprAST(std::shared_ptr<Token> token);
	ExprAST(const ExprAST &);
	ExprAST(ExprAST &&);

	virtual void print_dbg();
	virtual ~ExprAST();
};

struct IntExprAST: public ExprAST {
	int64_t value;

	IntExprAST(std::shared_ptr<Token> token);
	~IntExprAST() override;
	void print_dbg() override;
};

struct FloatExprAST: public ExprAST {
	double value;

	FloatExprAST(std::shared_ptr<Token> token);
	~FloatExprAST() override;
	void print_dbg() override;
};

struct VariableExprAST: public ExprAST {
	std::string name;
	std::string type;
	std::shared_ptr<Token> type_token;

	VariableExprAST(std::shared_ptr<Token> token);
	VariableExprAST(std::shared_ptr<Token> token, std::shared_ptr<Token> type_token);
	~VariableExprAST() override;
	void print_dbg() override;
};

struct BinaryExprAST: public ExprAST {
	TOKEN_TYPE op;
	std::shared_ptr<ExprAST> lefthand, righthand;

	BinaryExprAST(std::shared_ptr<Token> token, std::shared_ptr<ExprAST> lefthand, std::shared_ptr<ExprAST> righthand);
	~BinaryExprAST() override;
	void print_dbg() override;
};

struct CallExprAST: public ExprAST {
	std::string callee;
	std::vector<std::shared_ptr<ExprAST>> args;

	CallExprAST(std::shared_ptr<Token> token);
	~CallExprAST() override;
	void add_arg(std::shared_ptr<ExprAST> arg);
	void print_dbg() override;
};

struct FunctionArg {
	std::shared_ptr<Token> token;
	std::shared_ptr<Token> type_token;
	std::string name;
	std::string type;

	FunctionArg(std::shared_ptr<Token> token, std::shared_ptr<Token> type_token);
	void print_dbg();
};

// Function prototype
struct PrototypeAST {
	std::string name;
	std::vector<std::shared_ptr<FunctionArg>> args;

	PrototypeAST(const std::string &name);
	~PrototypeAST();

	void add_arg(std::shared_ptr<FunctionArg>);
	void print_dbg();
};

// Represents a whole function
struct FunctionAST: public ExprAST {
	std::unique_ptr<PrototypeAST> proto;
	std::shared_ptr<ExprAST> body;

	FunctionAST(std::shared_ptr<Token> _token, std::unique_ptr<PrototypeAST> proto, std::shared_ptr<ExprAST> body);
	~FunctionAST() override;
	void print_dbg() override;
};

} // namespace LibOrichalcum
