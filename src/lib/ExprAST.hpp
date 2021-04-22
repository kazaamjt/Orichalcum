#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "Lexer.hpp"

namespace LibOrichalcum {

struct ExprAST {
	std::unique_ptr<Token> token;
	ExprAST(const Token &token);
	ExprAST(const ExprAST &);
	ExprAST(ExprAST &&);

	virtual void print_dbg();
	virtual ~ExprAST();
};

struct IntExprAST: public ExprAST {
	int64_t value;

	IntExprAST(const Token &token);
	~IntExprAST() override;
	void print_dbg() override;
};

struct FloatExprAST: public ExprAST {
	double value;

	FloatExprAST(const Token &token);
	~FloatExprAST() override;
	void print_dbg() override;
};

struct VariableExprAST: public ExprAST {
	std::string name;
	std::string type;

	VariableExprAST(const Token &token);
	VariableExprAST(const Token &token, const std::string &type);
	~VariableExprAST() override;
	void print_dbg() override;
};

struct BinaryExprAST: public ExprAST {
	TOKEN_TYPE op;
	std::unique_ptr<ExprAST> lefthand, righthand;

	BinaryExprAST(const Token &token, std::unique_ptr<ExprAST> lefthand, std::unique_ptr<ExprAST> righthand);
	~BinaryExprAST() override;
	void print_dbg() override;
};

struct CallExprAST: public ExprAST {
	std::string callee;
	std::vector<ExprAST> args;

	CallExprAST(const Token &token, std::vector<ExprAST> args);
	~CallExprAST() override;
	void print_dbg() override;
};

struct FunctionArg {
	Token token;
	std::string name;
	std::string type;

	FunctionArg(const Token &_token, const std::string &type);
	void print_dbg();
};

// Function prototype
struct ProtoTypeAST {
	std::string name;
	std::vector<FunctionArg> args;

	ProtoTypeAST(const std::string &name, std::vector<FunctionArg> args);
	~ProtoTypeAST();
	void print_dbg();
};

// Represents a whole function
struct FunctionAST: public ExprAST {
	std::unique_ptr<ProtoTypeAST> proto;
	std::unique_ptr<ExprAST> body;

	FunctionAST(const Token &_token, std::unique_ptr<ProtoTypeAST> proto, std::unique_ptr<ExprAST> body);
	~FunctionAST() override;
	void print_dbg() override;
};

} // namespace LibOrichalcum
