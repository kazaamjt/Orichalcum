#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <variant>

#include "Lexer.hpp"

namespace LibOrichalcum {

struct ExprAST {
	virtual void print_dbg();
	virtual ~ExprAST();
};

struct IntExprAST: public ExprAST {
	int64_t value;

	IntExprAST(const std::string &value);
	void print_dbg();
};

struct FloatExprAST: public ExprAST {
	double value;

	FloatExprAST(const std::string &value);
	void print_dbg();
};

struct VariableExprAST: public ExprAST {
	std::string name;
	std::string type;

	VariableExprAST(const std::string &name, const std::string &type);
	void print_dbg();
};

struct BinaryExprAST: public ExprAST {
	std::unique_ptr<Token> op;
	std::unique_ptr<ExprAST> lefthand, righthand;

	BinaryExprAST(std::unique_ptr<Token> op, std::unique_ptr<ExprAST> lefthand, std::unique_ptr<ExprAST> righthand);
	void print_dbg();
};

struct CallExprAST: public ExprAST {
	std::string callee;
	std::vector<ExprAST> args;

	CallExprAST(const std::string callee, std::vector<ExprAST> args);
	void print_dbg();
};

struct FunctionArg {
	std::string name;
	std::string type;

	FunctionArg(const std::string &name, const std::string &type);
	void print_dbg();
};

// Function prototype
struct ProtoTypeAST {
	std::string name;
	std::vector<FunctionArg> args;

	ProtoTypeAST(const std::string &name, std::vector<FunctionArg> args);
	void print_dbg();
};

// Represents a whole function
struct FunctionAST: public ExprAST {
	std::unique_ptr<ProtoTypeAST> proto;
	std::unique_ptr<ExprAST> body;

	FunctionAST(std::unique_ptr<ProtoTypeAST> proto, std::unique_ptr<ExprAST> body);
	void print_dbg();
};

} // namespace LibOrichalcum
