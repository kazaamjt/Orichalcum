#include "Debug.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <variant>

#include "Misc.hpp"
#include "Log.hpp"

// TODO: Make sure calls to debug print functions only happen if debug is on

namespace LibOrichalcum::Debug {

void print_token(const Token &token) {
	Log::debug(
		"Token " + to_string(token.type)
		+ " \"" + token.content
		+ "\" " + token.file_path
		+ "(" + std::to_string(token.index.line + 1) + ","
		+ std::to_string(token.index.col + 1) + ")"
	);
}

void print_expr(ExprAST &expr_ast) {
	expr_ast.print_dbg();
}

static void print_line(int line) {
	static int last_line = 0;
	line += 1;
	std::cout << "line ";
	if (line != last_line) {
		std::cout << std::left << std::setfill(' ') << std::setw(5) << line;
		last_line = line;
	}
	else std::cout << "|    ";
}

void disassemble_chunk(std::shared_ptr<Chunk> chunk) {
	Log::debug("Disassembling chunk " + chunk->name);

	size_t i = 0;
	while (i < chunk->size()) {
		size_t next_i = disassemble_instruction(i, chunk);
		i = next_i;
	}
}

size_t disassemble_instruction(size_t index, std::shared_ptr<Chunk> chunk) {
	print_line(chunk->get_token(index)->index.line);
	OP_CODE op_code = chunk->get(index).op_code;
	switch (op_code) {
		case OP_CODE::RETURN: {
			std::cout<< Misc::to_hex(index) << " RETURN" << std::endl;
		} break;

		case OP_CODE::CONST_: {
			std::cout<< Misc::to_hex(index) << " CONST" << std::endl;
			size_t const_index = chunk->get(++index).index;
			print_line(chunk->get_token(index)->index.line);
			std::cout << Misc::to_hex(index) << " CONST_INDEX => ";
			std::cout << Misc::to_hex(const_index) << " = ";
			print_const(chunk->get_const(const_index));
			std::cout << std::endl;
		} break;

		case OP_CODE::NEGATE:
		case OP_CODE::ADD:
		case OP_CODE::SUBTRACT:
		case OP_CODE::MULTIPLY:
		case OP_CODE::DIVIDE:
		{
			std::cout<< Misc::to_hex(index) << " " << to_string(op_code) << std::endl;
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

std::string to_string(OP_CODE op_code) {
	switch (op_code) {
		case OP_CODE::RETURN: return "RETURN";
		case OP_CODE::CONST_: return "CONST";
		case OP_CODE::NEGATE: return "NEGATE";
		case OP_CODE::ADD: return "ADD";
		case OP_CODE::SUBTRACT: return "SUBTRACT";
		case OP_CODE::MULTIPLY: return "MULTIPLY";
		case OP_CODE::DIVIDE: return "DIVIDE";
	}
}

} // LibOrichalcum
