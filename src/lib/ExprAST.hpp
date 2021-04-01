#pragma once
#include <cstdint>
#include <memory>
#include <string>

#include "Lexer.hpp"

namespace LibOrichalcum {

class ExprAST {
public:
	virtual ~ExprAST();
};

class IntExprAST: public ExprAST {
public:
	int64_t value;

	IntExprAST(const std::string &value);
};

class FloatExprAST: public ExprAST {
public:
	double value;

	FloatExprAST(const std::string &value);
};

class VariableExprAST: public ExprAST {
public:
	std::string name;
	std::string type;

	VariableExprAST(const std::string &name, const std::string &type);
};

class BinaryExprAST : public ExprAST {
public:
	Token op;
	std::unique_ptr<ExprAST> lefthand, righthand;

	BinaryExprAST(Token op, std::unique_ptr<ExprAST> lefthand, std::unique_ptr<ExprAST> righthand);
};

class FunctionArg {
public:
	std::string name;
	std::string type;

	FunctionArg(const std::string &name, const std::string &type);
};

// Function prototype
class PrototypeAST {
public:
	std::string name;
	std::vector<FunctionArg> args;

	PrototypeAST(const std::string &name, std::vector<FunctionArg> args);
};

// Represents a whole function
class FunctionAST {
public:
	std::unique_ptr<PrototypeAST> proto;
	std::unique_ptr<ExprAST> body;

	FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body);
};

} // namespace LibOrichalcum
