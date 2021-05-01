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

	virtual void print_dbg(const std::string &pre = "");
	virtual ~ExprAST();
};

struct IntExprAST: public ExprAST {
	int64_t value;

	IntExprAST(std::shared_ptr<Token> token, bool print_debug);
	~IntExprAST() override;
	void print_dbg(const std::string &pre = "") override;
};

struct FloatExprAST: public ExprAST {
	double value;

	FloatExprAST(std::shared_ptr<Token> token, bool print_debug);
	~FloatExprAST() override;
	void print_dbg(const std::string &pre = "") override;
};

struct VariableExprAST: public ExprAST {
	std::string name;
	std::string type;
	std::shared_ptr<Token> type_token;

	VariableExprAST(std::shared_ptr<Token> token, bool print_debug);
	VariableExprAST(
		std::shared_ptr<Token> token,
		std::shared_ptr<Token> type_token,
		bool print_debug
	);
	~VariableExprAST() override;
	void print_dbg(const std::string &pre = "") override;
};

struct BinaryExprAST: public ExprAST {
	std::string op;
	std::shared_ptr<ExprAST> lefthand, righthand;

	BinaryExprAST(
		std::shared_ptr<Token> token,
		std::shared_ptr<ExprAST> lefthand,
		std::shared_ptr<ExprAST> righthand,
		bool print_debug
	);
	~BinaryExprAST() override;
	void print_dbg(const std::string &pre = "") override;
};

struct CallExprAST: public ExprAST {
	std::string callee;
	std::vector<std::shared_ptr<ExprAST>> args;

	CallExprAST(
		std::shared_ptr<Token> token,
		std::vector<std::shared_ptr<ExprAST>> args,
		bool print_debug
	);
	~CallExprAST() override;
	void print_dbg(const std::string &pre = "") override;
};

struct FunctionArg {
	std::shared_ptr<Token> token;
	std::shared_ptr<Token> type_token;
	std::string name;
	std::string type;

	FunctionArg(
		std::shared_ptr<Token> token,
		std::shared_ptr<Token> type_token,
		bool print_debug
	);
	void print_dbg(const std::string &pre = "");
};

// Function prototype
struct PrototypeAST {
	std::string name;
	std::vector<std::shared_ptr<FunctionArg>> args;
	std::string return_type;
	std::shared_ptr<Token> return_token;

	PrototypeAST(
		const std::string &name,
		std::vector<std::shared_ptr<FunctionArg>> args,
		std::shared_ptr<Token> return_token,
		bool print_debug
	);
	~PrototypeAST();

	void print_dbg(const std::string &pre = "");
};

struct TopLevelExprAST: public ExprAST {
	std::shared_ptr<ExprAST> body;

	TopLevelExprAST(
		std::shared_ptr<Token> token,
		std::shared_ptr<ExprAST> body,
		bool print_debug
	);
	~TopLevelExprAST() override;
	void print_dbg(const std::string &pre = "") override;
};

// Represents a whole function
struct FunctionAST: public ExprAST {
	std::shared_ptr<PrototypeAST> proto;
	std::vector<std::shared_ptr<TopLevelExprAST>> body;

	FunctionAST(
		std::shared_ptr<Token> token,
		std::shared_ptr<PrototypeAST> proto,
		std::vector<std::shared_ptr<TopLevelExprAST>> body,
		bool print_debug
	);
	~FunctionAST() override;
	void print_dbg(const std::string &pre = "") override;
};

struct PassExprAST: public ExprAST {
	PassExprAST(
		std::shared_ptr<Token> token,
		bool print_debug
	);
	~PassExprAST() override;
	void print_dbg(const std::string &pre = "") override;
};

struct UnaryNegExprAST: public ExprAST {
	std::shared_ptr<ExprAST> rhs;

	UnaryNegExprAST(
		std::shared_ptr<Token> token,
		std::shared_ptr<ExprAST> rhs,
		bool print_debug
	);
	~UnaryNegExprAST() override;
	void print_dbg(const std::string &pre = "") override;
};

} // namespace LibOrichalcum
