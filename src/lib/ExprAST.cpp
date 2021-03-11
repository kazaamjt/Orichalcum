#include "ExprAST.hpp"

#include <sstream>

namespace OrichalcumLib {

IntExprAST::IntExprAST(const std::string &_value) {
	std::stringstream stream(_value);
	stream >> value;
}

FloatExprAST::FloatExprAST(const std::string &_value) {
	std::stringstream stream(_value);
	stream >> value;
}

VariableExprAST::VariableExprAST(const std::string &_name, const std::string &_type):
	name(_name), type(_type) { }

BinaryExprAST::BinaryExprAST(
	Token _op,
	std::unique_ptr<ExprAST> lhs,
	std::unique_ptr<ExprAST> rhs
): op(_op), lefthand(std::move(lhs)), righthand(std::move(rhs)) { }

FunctionArg::FunctionArg(const std::string &_name, const std::string &_type) :
	name(_name), type(_type) { }

PrototypeAST::PrototypeAST(const std::string &_name, std::vector<FunctionArg> _args):
	name(_name), args(_args) { }

FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> _proto, std::unique_ptr<ExprAST> _body):
	proto(std::move(_proto)), body(std::move(_body)) { }

} // namespace OrichalcumLib
