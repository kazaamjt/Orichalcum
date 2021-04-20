#include "ExprAST.hpp"

#include <sstream>

#include "Log.hpp"

namespace LibOrichalcum {

void ExprAST::print_dbg() {
	Log::error("Debug printer got passed object with type base ExprAST, this object shouldn't exist?!");
}

IntExprAST::IntExprAST(const std::string &_value) {
	std::stringstream stream(_value);
	stream >> value;
}

void IntExprAST::print_dbg() {
	Log::debug("IntExprAST " + std::to_string(value));
}

FloatExprAST::FloatExprAST(const std::string &_value) {
	std::stringstream stream(_value);
	stream >> value;
}

void FloatExprAST::print_dbg() {
	Log::debug("FloatExprAST " + std::to_string(value));
}

VariableExprAST::VariableExprAST(const std::string &_name, const std::string &_type):
	name(_name), type(_type) { }

void VariableExprAST::print_dbg() {
	Log::debug(
		"VariableExprAST {name: \"" + name
		+ "\", type: \"" + type + "\"}"
	);
}

BinaryExprAST::BinaryExprAST(
	std::unique_ptr<Token> _op,
	std::unique_ptr<ExprAST> lhs,
	std::unique_ptr<ExprAST> rhs):
	op(std::move(_op)), lefthand(std::move(lhs)), righthand(std::move(rhs)) { }

void BinaryExprAST::print_dbg() {
	Log::debug("BinaryExprAST: " + to_string(op->type));
	Log::debug("Left hand: ");
	lefthand->print_dbg();
	Log::debug("Right hand: ");
	righthand->print_dbg();
}

FunctionArg::FunctionArg(const std::string &_name, const std::string &_type) :
	name(_name), type(_type) { }

void FunctionArg::print_dbg() {
	Log::debug(
		"FunctionArg {name: \"" + name
		+ "\", type: \"" + type + "\"}"
	);
}

ProtoTypeAST::ProtoTypeAST(const std::string &_name, std::vector<FunctionArg> _args):
	name(_name), args(_args) { }

void ProtoTypeAST::print_dbg() {
	Log::debug("PrototypeAST: " + name);
	for (auto arg: args) {
		arg.print_dbg();
	}
}

FunctionAST::FunctionAST(
	std::unique_ptr<ProtoTypeAST> _proto,
	std::unique_ptr<ExprAST> _body):
	proto(std::move(_proto)), body(std::move(_body)) { }

void FunctionAST::print_dbg() {
	Log::debug("FunctionAST: ");
	proto->print_dbg();
	Log::debug("Body:");
	body->print_dbg();
}

} // namespace LibOrichalcum
