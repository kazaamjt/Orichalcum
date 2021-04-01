#include "Debug.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "Misc.hpp"
#include "Log.hpp"

namespace LibOrichalcum::Debug {

void print_token(const Token &token) {
	Log::debug(
		"(" + std::to_string(token.index.line + 1) + ","
		+ std::to_string(token.index.col + 1) + ") "
		+ to_string(token.type) + " " + token.content
	);
}

static void print_line(int line) {
	static int last_line = 0;
	std::cout << "line ";
	if (line != last_line) {
		std::cout << std::left << std::setfill(' ') << std::setw(5) << line;
		last_line = line;
	}
	else std::cout << "|    ";
}

void disassemble_chunk(Chunk &chunk) {
	Log::debug("Disassembling chunk " + chunk.name);

	size_t i = 0;
	while (i < chunk.size()) {
		size_t next_i = disassemble_instruction(i, chunk);
		i = next_i;
	}
}

size_t disassemble_instruction(size_t index, Chunk &chunk) {
	print_line(chunk.get_line(index));
	switch (chunk.get(index).op_code) {
		case OP_CODE::RETURN: {
			std::cout<< Misc::to_hex(index) << " RETURN" << std::endl;
		} break;

		case OP_CODE::CONST: {
			std::cout<< Misc::to_hex(index) << " CONST_INT" << std::endl;
			size_t const_index = chunk.get(++index).index;
			print_line(chunk.get_line(index));
			std::cout << Misc::to_hex(index) << " CONST_INDEX => ";
			std::cout << Misc::to_hex(const_index) << " = ";
			print_const(chunk.get_const(const_index));
			std::cout << std::endl;
		} break;

		case OP_CODE::NEGATE: {
			std::cout<< Misc::to_hex(index) << " NEGATE" << std::endl;
		} break;
		case OP_CODE::ADD: {
			std::cout<< Misc::to_hex(index) << " ADD" << std::endl;
		} break;
		case OP_CODE::SUBTRACT: {
			std::cout<< Misc::to_hex(index) << " SUBTRACT" << std::endl;
		} break;
		case OP_CODE::MULTIPLY: {
			std::cout<< Misc::to_hex(index) << " MULTIPLY" << std::endl;
		} break;
		case OP_CODE::DIVIDE: {
			std::cout<< Misc::to_hex(index) << " DIVIDE" << std::endl;
		} break;
	}

	return ++index;
}

std::string to_string(const Constant &constant) {
	switch (constant.type) {
		case CONSTANT_TYPE::INT:
			return std::to_string(constant.value.int_);
		case CONSTANT_TYPE::FLOAT:
			return std::to_string(constant.value.float_);
	}
}

void print_const(const Constant &constant) {
	std::cout << to_string(constant);
}

void print_stack(ConstStack &stack) {
	Log::debug("Current stack:");
	std::cout << "[";
	for (Constant constant: stack.internal) {
		print_const(constant);
		std::cout << " ";
	}
	if (stack.internal.size() > 0) {
		std::cout << "\b]" << std::endl;
	} else {
		std::cout << " ]" << std::endl;
	}
}

std::string to_string(const TOKEN_TYPE token_type) {
	switch(token_type){
		case TOKEN_TYPE::EMPTY: return "EMPTY";
		case TOKEN_TYPE::EOF_TOKEN: return "EOF_TOKEN";
		case TOKEN_TYPE::INDENT: return "INDENT";
		case TOKEN_TYPE::LEFT_PAREN: return "LEFT_PAREN";
		case TOKEN_TYPE::RIGHT_PAREN: return "RIGHT_PAREN";
		case TOKEN_TYPE::COMMA: return "COMMA";
		case TOKEN_TYPE::DOT: return "DOT";
		case TOKEN_TYPE::ASIGNMENT_OP: return "ASIGNMENT_OP";
		case TOKEN_TYPE::ARITHMATIC_OP: return "ARITHMATIC_OP";
		case TOKEN_TYPE::COMPARISON_OP: return "COMPARISON_OP";
		case TOKEN_TYPE::NOT_OP: return "NOT_OP";
		case TOKEN_TYPE::FLOAT: return "FLOAT";
		case TOKEN_TYPE::INTEGER: return "INTEGER";
		case TOKEN_TYPE::AND: return "AND";
		case TOKEN_TYPE::AS: return "AS";
		case TOKEN_TYPE::ASSERT: return "ASSERT";
		case TOKEN_TYPE::ASYNC: return "ASYNC";
		case TOKEN_TYPE::AWAIT: return "AWAIT";
		case TOKEN_TYPE::BREAK: return "BREAK";
		case TOKEN_TYPE::CONSTRAINT: return "CONSTRAINT";
		case TOKEN_TYPE::CONTINUE: return "CONTINUE";
		case TOKEN_TYPE::CLASS: return "CLASS";
		case TOKEN_TYPE::DEF: return "DEF";
		case TOKEN_TYPE::DEL: return "DEL";
		case TOKEN_TYPE::ELIF: return "ELIF";
		case TOKEN_TYPE::ELSE: return "ELSE";
		case TOKEN_TYPE::EXCEPT: return "EXCEPT";
		case TOKEN_TYPE::FALSE: return "FALSE";
		case TOKEN_TYPE::FINALLY: return "FINALLY";
		case TOKEN_TYPE::FOR: return "FOR";
		case TOKEN_TYPE::FROM: return "FROM";
		case TOKEN_TYPE::GLOBAL: return "GLOBAL";
		case TOKEN_TYPE::IF: return "IF";
		case TOKEN_TYPE::IMPORT: return "IMPORT";
		case TOKEN_TYPE::IN: return "IN";
		case TOKEN_TYPE::IS: return "IS";
		case TOKEN_TYPE::LAMBDA: return "LAMBDA";
		case TOKEN_TYPE::NONE: return "NONE";
		case TOKEN_TYPE::NONLOCAL: return "NONLOCAL";
		case TOKEN_TYPE::NOT: return "NOT";
		case TOKEN_TYPE::OR: return "OR";
		case TOKEN_TYPE::PASS: return "PASS";
		case TOKEN_TYPE::RAISE: return "RAISE";
		case TOKEN_TYPE::RETURN: return "RETURN";
		case TOKEN_TYPE::STRUCT: return "STRUCT";
		case TOKEN_TYPE::TRUE: return "TRUE";
		case TOKEN_TYPE::TRY: return "TRY";
		case TOKEN_TYPE::WHILE: return "WHILE";
		case TOKEN_TYPE::WITH: return "WITH";
		case TOKEN_TYPE::YIELD: return "YIELD";
		case TOKEN_TYPE::RETURN_TYPE_SIGN: return "RETURN_TYPE_SIGN";
		case TOKEN_TYPE::IDENTIFIER: return "IDENTIFIER";
		case TOKEN_TYPE::UNKNOWN: return "UNKNOWN";
		case TOKEN_TYPE::STRING: return "STRING";
	}
}

} // LibOrichalcum
