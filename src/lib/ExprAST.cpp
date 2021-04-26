#include "ExprAST.hpp"

#include <sstream>

#include "Log.hpp"

namespace LibOrichalcum {

ExprAST::ExprAST(std::shared_ptr<Token> _token):
token(_token) { }

ExprAST::ExprAST(const ExprAST &expr):
token(new Token(*expr.token)) { }

void ExprAST::print_dbg() {
	Log::error("Debug printer got passed object with type base ExprAST, this object shouldn't exist?!");
}

ExprAST::~ExprAST() { }

IntExprAST::IntExprAST(std::shared_ptr<Token> _token):
ExprAST(_token) {
	std::stringstream stream(token->content);
	stream >> value;
}

void IntExprAST::print_dbg() {
	Log::debug("IntExprAST " + std::to_string(value));
}

IntExprAST::~IntExprAST() { }

FloatExprAST::FloatExprAST(std::shared_ptr<Token> _token):
ExprAST(_token) {
	std::stringstream stream(token->content);
	stream >> value;
}

void FloatExprAST::print_dbg() {
	Log::debug("FloatExprAST " + std::to_string(value));
}

FloatExprAST::~FloatExprAST() { }

VariableExprAST::VariableExprAST(std::shared_ptr<Token> _token):
ExprAST(_token),
name(_token->content) { }

VariableExprAST::VariableExprAST(
	std::shared_ptr<Token> _token,
	std::shared_ptr<Token> _type_token):
ExprAST(_token),
name(_token->content),
type(_type_token->content),
type_token(_type_token) { }

void VariableExprAST::print_dbg() {
	Log::debug(
		"VariableExprAST {name: \"" + name
		+ "\", type: \"" + type + "\"}"
	);
}

VariableExprAST::~VariableExprAST() { }

BinaryExprAST::BinaryExprAST(
	std::shared_ptr<Token> _token,
	std::shared_ptr<ExprAST> lhs,
	std::shared_ptr<ExprAST> rhs):
ExprAST(_token),
op(_token->type),
lefthand(std::move(lhs)),
righthand(std::move(rhs)) { }

void BinaryExprAST::print_dbg() {
	Log::debug("BinaryExprAST: " + to_string(op));
	Log::debug("Left hand: ");
	lefthand->print_dbg();
	Log::debug("Right hand: ");
	righthand->print_dbg();
}

BinaryExprAST::~BinaryExprAST() { }

CallExprAST::CallExprAST(std::shared_ptr<Token> _token):
ExprAST(_token),
callee(_token->content) { }

void CallExprAST::add_arg(std::shared_ptr<ExprAST> arg) {
	args.push_back(arg);
}

void CallExprAST::print_dbg() {
	Log::debug("CallExprAST: Call to " + callee);
	for (auto arg: args) {
		arg->print_dbg();
	}
}

CallExprAST::~CallExprAST() { }

FunctionArg::FunctionArg(std::shared_ptr<Token> _token, std::shared_ptr<Token> _type_token) :
token(_token), type_token(_type_token), name(_token->content), type(_type_token->content) { }

void FunctionArg::print_dbg() {
	Log::debug(
		"FunctionArg {name: \"" + name
		+ "\", type: \"" + type + "\"}"
	);
}

PrototypeAST::PrototypeAST(const std::string &_name):
	name(_name) { }

void PrototypeAST::add_arg(std::shared_ptr<FunctionArg> arg) {
	args.push_back(arg);
}

void PrototypeAST::print_dbg() {
	Log::debug("PrototypeAST: " + name);
	for (auto arg: args) {
		arg->print_dbg();
	}
}

PrototypeAST::~PrototypeAST() { }

FunctionAST::FunctionAST(
	std::shared_ptr<Token> _token,
	std::unique_ptr<PrototypeAST> _proto,
	std::shared_ptr<ExprAST> _body):
ExprAST(_token),
proto(std::move(_proto)),
body(_body) { }

FunctionAST::~FunctionAST() { }

void FunctionAST::print_dbg() {
	Log::debug("FunctionAST: ");
	proto->print_dbg();
	Log::debug("Body:");
	body->print_dbg();
}

} // namespace LibOrichalcum
