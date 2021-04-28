#include "ExprAST.hpp"

#include <sstream>

#include "Log.hpp"

namespace LibOrichalcum {

ExprAST::ExprAST(std::shared_ptr<Token> _token):
token(_token) { }

ExprAST::ExprAST(const ExprAST &expr):
token(new Token(*expr.token)) { }

void ExprAST::print_dbg(const std::string &pre) {
	Log::error(pre);
	throw Error(
		COMPILE_RESULT::PARSER_ERROR,
		"Debug printer got passed object with type ExprAST, this object shouldn't exist, it represents nothing concrete... this is BadBadNotGood.",
		token
	);
}

ExprAST::~ExprAST() { }

IntExprAST::IntExprAST(std::shared_ptr<Token> _token, bool print_debug):
ExprAST(_token) {
	std::stringstream stream(token->content);
	stream >> value;
	if (print_debug) print_dbg("Created ");
}

void IntExprAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "IntExprAST " + std::to_string(value));
}

IntExprAST::~IntExprAST() { }

FloatExprAST::FloatExprAST(std::shared_ptr<Token> _token, bool print_debug):
ExprAST(_token) {
	std::stringstream stream(token->content);
	stream >> value;
	if (print_debug) print_dbg("Created ");
}

void FloatExprAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "FloatExprAST " + std::to_string(value));
}

FloatExprAST::~FloatExprAST() { }

VariableExprAST::VariableExprAST(std::shared_ptr<Token> _token, bool print_debug):
ExprAST(_token),
name(_token->content) {
	if (print_debug) print_dbg("Created ");
}

VariableExprAST::VariableExprAST(
	std::shared_ptr<Token> _token,
	std::shared_ptr<Token> _type_token,
	bool print_debug):
ExprAST(_token),
name(_token->content),
type(_type_token->content),
type_token(_type_token) {
	if (print_debug) print_dbg("Created ");
}

void VariableExprAST::print_dbg(const std::string &pre) {
	std::string output_str = pre + "VariableExprAST {name: \"" + name;
	if (!type.empty()) {
		output_str += + "\", type: \"" + type;
	}
	output_str += "\"}";
	Log::debug(output_str);
}

VariableExprAST::~VariableExprAST() { }

BinaryExprAST::BinaryExprAST(
	std::shared_ptr<Token> _token,
	std::shared_ptr<ExprAST> lhs,
	std::shared_ptr<ExprAST> rhs,
	bool print_debug):
ExprAST(_token),
op(_token->type),
lefthand(std::move(lhs)),
righthand(std::move(rhs)) {
	if (print_debug) print_dbg("Created ");
}

void BinaryExprAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "BinaryExprAST: " + to_string(op));
	Log::debug("Left hand: ");
	lefthand->print_dbg();
	Log::debug("Right hand: ");
	righthand->print_dbg();
}

BinaryExprAST::~BinaryExprAST() { }

CallExprAST::CallExprAST(
	std::shared_ptr<Token> _token,
	std::vector<std::shared_ptr<ExprAST>> _args,
	bool print_debug):
ExprAST(_token),
callee(_token->content),
args(_args) {
	if (print_debug) print_dbg("Created ");
}

void CallExprAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "CallExprAST: Call to " + callee);
	for (auto arg: args) {
		arg->print_dbg();
	}
}

CallExprAST::~CallExprAST() { }

FunctionArg::FunctionArg(
	std::shared_ptr<Token> _token,
	std::shared_ptr<Token> _type_token,
	bool print_debug):
token(_token),
type_token(_type_token),
name(_token->content),
type(_type_token->content) {
	if (print_debug) print_dbg("Created ");
}

void FunctionArg::print_dbg(const std::string &pre) {
	Log::debug(
		pre + "FunctionArg {name: \"" + name
		+ "\", type: \"" + type + "\"}"
	);
}

PrototypeAST::PrototypeAST(
	const std::string &_name,
	std::vector<std::shared_ptr<FunctionArg>> _args,
	bool print_debug):
name(_name),
args(_args) {
	if (print_debug) print_dbg("Created ");
}

void PrototypeAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "PrototypeAST: " + name);
	for (auto arg: args) {
		arg->print_dbg();
	}
}

PrototypeAST::~PrototypeAST() { }

FunctionAST::FunctionAST(
	std::shared_ptr<Token> _token,
	std::unique_ptr<PrototypeAST> _proto,
	std::shared_ptr<ExprAST> _body,
	bool print_debug):
ExprAST(_token),
proto(std::move(_proto)),
body(_body) {
	if (print_debug) print_dbg("Created ");
}

FunctionAST::~FunctionAST() { }

void FunctionAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "FunctionAST: ");
	proto->print_dbg();
	Log::debug("Body:");
	body->print_dbg();
}

} // namespace LibOrichalcum
