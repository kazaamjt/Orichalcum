#include "ExprAST.hpp"

#include <sstream>

#include "Log.hpp"
#include "Debug.hpp"

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

void ExprAST::compile(std::shared_ptr<Chunk> chunk) {
	throw Error(
		COMPILE_RESULT::PARSER_ERROR,
		"Compiler got passed object with type ExprAST, this object shouldn't exist, it represents nothing concrete... this is BadBadNotGood.",
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

void IntExprAST::compile(std::shared_ptr<Chunk> chunk) {
	chunk->write(value, token);
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

void FloatExprAST::compile(std::shared_ptr<Chunk> chunk) {
	chunk->write(value, token);
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

void VariableExprAST::compile(std::shared_ptr<Chunk> chunk) {
	throw Error(
		COMPILE_RESULT::COMPILER_ERROR,
		"VariableExprAST isn't currently implemented :(",
		token
	);
}

VariableExprAST::~VariableExprAST() { }

BinaryExprAST::BinaryExprAST(
	OP_CODE _op,
	std::shared_ptr<Token> _token,
	std::shared_ptr<ExprAST> lhs,
	std::shared_ptr<ExprAST> rhs,
	bool print_debug):
ExprAST(_token),
op(_op),
lefthand(std::move(lhs)),
righthand(std::move(rhs)) {
	if (print_debug) print_dbg("Created ");
}

void BinaryExprAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "BinaryExprAST: " + Debug::to_string(op));
	Log::debug("Left hand: ");
	lefthand->print_dbg();
	Log::debug("Right hand: ");
	righthand->print_dbg();
}

void BinaryExprAST::compile(std::shared_ptr<Chunk> chunk) {
	lefthand->compile(chunk);
	righthand->compile(chunk);
	chunk->write(op, token);
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
	int i = 0;
	for (auto arg: args) {
		Log::debug("arg " + std::to_string(++i) + ":");
		arg->print_dbg();
	}
}

void CallExprAST::compile(std::shared_ptr<Chunk> chunk) {
	throw Error(
		COMPILE_RESULT::COMPILER_ERROR,
		"CallExprAST isn't currently implemented :(",
		token
	);
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
	std::shared_ptr<Token> _return_token,
	bool print_debug):
name(_name),
args(_args),
return_type(_return_token->content),
return_token(_return_token) {
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
	std::shared_ptr<PrototypeAST> _proto,
	std::vector<std::shared_ptr<TopLevelExprAST>> _body,
	bool print_debug):
ExprAST(_token),
proto(_proto),
body(_body) {
	if (print_debug) print_dbg("Created ");
}

void FunctionAST::compile(std::shared_ptr<Chunk> chunk) {
	throw Error(
		COMPILE_RESULT::COMPILER_ERROR,
		"FunctionAST isn't currently implemented :(",
		token
	);
}

FunctionAST::~FunctionAST() { }

void FunctionAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "FunctionAST: ");
	proto->print_dbg();
	Log::debug("Body:");
	for (auto expr: body) {
		expr->print_dbg();
	}
}

TopLevelExprAST::TopLevelExprAST(
	std::shared_ptr<Token> _token,
	std::shared_ptr<ExprAST> _body,
	bool print_debug):
ExprAST(_token),
body(_body) {
	if (print_debug) print_dbg("Created ");
}

void TopLevelExprAST::compile(std::shared_ptr<Chunk> chunk) {
	body->compile(chunk);
}

void TopLevelExprAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "TopLevelExprAST: ");
	body->print_dbg();
}

TopLevelExprAST::~TopLevelExprAST() { }

PassExprAST::PassExprAST(
	std::shared_ptr<Token> _token,
	bool print_debug):
ExprAST(_token) {
	if (print_debug) print_dbg("Created ");
}

void PassExprAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "PassExprAST");
}

void PassExprAST::compile(std::shared_ptr<Chunk> chunk) {
	throw Error(
		COMPILE_RESULT::COMPILER_ERROR,
		"PassExprAST isn't currently implemented :(",
		token
	);
}

PassExprAST::~PassExprAST() { }

UnaryNegExprAST::UnaryNegExprAST(
	std::shared_ptr<Token> _token,
	std::shared_ptr<ExprAST> _rhs,
	bool print_debug
):
ExprAST(_token),
rhs(_rhs) {
	if (print_debug) print_dbg("Created ");
}

void UnaryNegExprAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "UnaryNegExprAST:");
	rhs->print_dbg();
}

void UnaryNegExprAST::compile(std::shared_ptr<Chunk> chunk) {
	rhs->compile(chunk);
	chunk->write(OP_CODE::NEGATE, token);
}

UnaryNegExprAST::~UnaryNegExprAST() { }

NoneExprAST::NoneExprAST(
	std::shared_ptr<Token> _token,
	bool print_debug):
ExprAST(_token) {
	if (print_debug) print_dbg("Created ");
}

void NoneExprAST::compile(std::shared_ptr<Chunk> chunk) {
	throw Error(
		COMPILE_RESULT::COMPILER_ERROR,
		"NoneExprAST isn't currently implemented :(",
		token
	);
}

void NoneExprAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "NoneExprAST.");
}

NoneExprAST::~NoneExprAST() { }

BoolExprAST::BoolExprAST(
	std::shared_ptr<Token> _token,
	bool print_debug):
ExprAST(_token) {
	if (token->type == TOKEN_TYPE::TRUE_) value = true;
	else if (token->type == TOKEN_TYPE::FALSE_) value = false;
	if (print_debug) print_dbg("Created ");
}

void BoolExprAST::compile(std::shared_ptr<Chunk> chunk) {
	throw Error(
		COMPILE_RESULT::COMPILER_ERROR,
		"BoolExprAST isn't currently implemented :(",
		token
	);
}

void BoolExprAST::print_dbg(const std::string &pre) {
	std::string val;
	if (value) val = "True";
	else val = "False";
	Log::debug(pre + "BoolExprAST. (" + val + ")");
}

BoolExprAST::~BoolExprAST() { }

EOFExprAST::EOFExprAST(
	std::shared_ptr<Token> _token,
	bool print_debug):
ExprAST(_token) {
	if (print_debug) print_dbg("Created ");
}

void EOFExprAST::compile(std::shared_ptr<Chunk> chunk) {
	throw Error(
		COMPILE_RESULT::COMPILER_ERROR,
		"Tried to compile an EOFExprAST, ",
		token
	);
}

void EOFExprAST::print_dbg(const std::string &pre) {
	Log::debug(pre + "EOFExprAST.");
}

EOFExprAST::~EOFExprAST() { }

} // namespace LibOrichalcum
