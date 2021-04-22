#include "ExprAST.hpp"

#include <sstream>

#include "Log.hpp"

namespace LibOrichalcum {

ExprAST::ExprAST(const Token &_token):
token(new Token(_token)) { }

ExprAST::ExprAST(const ExprAST &expr):
token(new Token(*expr.token)) { }

void ExprAST::print_dbg() {
	Log::error("Debug printer got passed object with type base ExprAST, this object shouldn't exist?!");
}

ExprAST::~ExprAST() { }

IntExprAST::IntExprAST(const Token &_token):
ExprAST(_token) {
	std::stringstream stream(token->content);
	stream >> value;
}

IntExprAST::~IntExprAST() { }

void IntExprAST::print_dbg() {
	Log::debug("IntExprAST " + std::to_string(value));
}

FloatExprAST::FloatExprAST(const Token & _token):
ExprAST(_token) {
	token = std::make_unique<Token>(_token);
	std::stringstream stream(token->content);
	stream >> value;
}

FloatExprAST::~FloatExprAST() { }

void FloatExprAST::print_dbg() {
	Log::debug("FloatExprAST " + std::to_string(value));
}

VariableExprAST::VariableExprAST(const Token &_token):
ExprAST(_token),
name(_token.content) {
	token = std::make_unique<Token>(_token);
}

VariableExprAST::VariableExprAST(
	const Token &_token,
	const std::string &_type):
ExprAST(_token),
name(_token.content),
type(_type) {
	token = std::make_unique<Token>(_token);
}

VariableExprAST::~VariableExprAST() { }

void VariableExprAST::print_dbg() {
	Log::debug(
		"VariableExprAST {name: \"" + name
		+ "\", type: \"" + type + "\"}"
	);
}

BinaryExprAST::BinaryExprAST(
	const Token &_token,
	std::unique_ptr<ExprAST> lhs,
	std::unique_ptr<ExprAST> rhs):
ExprAST(_token),
op(_token.type),
lefthand(std::move(lhs)),
righthand(std::move(rhs)) {
	token = std::make_unique<Token>(_token);
}

BinaryExprAST::~BinaryExprAST() { }

void BinaryExprAST::print_dbg() {
	Log::debug("BinaryExprAST: " + to_string(op));
	Log::debug("Left hand: ");
	lefthand->print_dbg();
	Log::debug("Right hand: ");
	righthand->print_dbg();
}

CallExprAST::CallExprAST(
	const Token &_token,
	std::vector<ExprAST> _args):
args(_args),
ExprAST(_token),
callee(_token.content) { }

CallExprAST::~CallExprAST() { }

void CallExprAST::print_dbg() {
	Log::debug("CallExprAST: Call to " + callee);
	for (auto arg: args) {
		arg.print_dbg();
	}
}

FunctionArg::FunctionArg(const Token &_token, const std::string &_type) :
token(_token), name(_token.content), type(_type) { }

void FunctionArg::print_dbg() {
	Log::debug(
		"FunctionArg {name: \"" + name
		+ "\", type: \"" + type + "\"}"
	);
}

ProtoTypeAST::ProtoTypeAST(const std::string &_name, std::vector<FunctionArg> _args):
	name(_name), args(_args) { }

ProtoTypeAST::~ProtoTypeAST() { }

void ProtoTypeAST::print_dbg() {
	Log::debug("PrototypeAST: " + name);
	for (auto arg: args) {
		arg.print_dbg();
	}
}

FunctionAST::FunctionAST(
	const Token &_token,
	std::unique_ptr<ProtoTypeAST> _proto,
	std::unique_ptr<ExprAST> _body):
ExprAST(_token),
proto(std::move(_proto)),
body(std::move(_body)) {
	token = std::make_unique<Token>(_token);
}

FunctionAST::~FunctionAST() { }

void FunctionAST::print_dbg() {
	Log::debug("FunctionAST: ");
	proto->print_dbg();
	Log::debug("Body:");
	body->print_dbg();
}

} // namespace LibOrichalcum
