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
	std::string log_str = "Token " + to_string(token.type);
	log_str += " \"" + token.content;
	log_str += "\" " + token.file_path;

	if (token.type != TOKEN_TYPE::EOF_TOKEN) {
		log_str += "(" + std::to_string(token.index.line + 1) + ",";
		log_str += std::to_string(token.index.col + 1) + ")";
	}
	Log::debug(log_str);
}

void print_expr(ExprAST &expr_ast) {
	expr_ast.print_dbg();
}

static void print_line(int line) {
	static int last_line = 0;
	if (line != last_line) {
		std::cout << "line ";
		std::cout << std::left << std::setfill(' ') << std::setw(5) << line;
		last_line = line;
	}
	else std::cout << "     |    ";
}

void disassemble_chunk(std::shared_ptr<Chunk> chunk) {
	Log::debug("Disassembling chunk " + chunk->name);

	size_t i = 0;
	while (i < chunk->size()) {
		disassemble_instruction(chunk, i);
		i++;
	}
}

void disassemble_instruction(std::shared_ptr<Chunk> chunk, size_t index) {
	static int line = 0;
	Instruction instruction = chunk->get(index);

	if (instruction.op_code == OP_CODE::CONST)
		line = chunk->get_const(instruction.index).token->index.line;
	else if (instruction.has_token)
		line = instruction.token->index.line;

	print_line(line);

	switch (instruction.op_code) {
		case OP_CODE::CONST: {
			std::cout<< Misc::to_hex(index) << " CONST ";
			std::cout << Misc::to_hex(instruction.index) << " = ";
			print_const(chunk->get_const(instruction.index));
			std::cout << std::endl;
		} break;

		case OP_CODE::RETURN:
		case OP_CODE::NEGATE:
		case OP_CODE::NOT:
		case OP_CODE::ADD:
		case OP_CODE::SUBTRACT:
		case OP_CODE::MULTIPLY:
		case OP_CODE::DIVIDE:
		case OP_CODE::INT_DIVIDE:
		case OP_CODE::EXPONENTIATION: {
			std::cout<< Misc::to_hex(index) << " " << to_string(instruction.op_code) << std::endl;
		} break;
	}
}

std::string to_string(const OrValue &constant) {
	switch (constant.type) {
		case OrValueType::INT:
			return std::to_string(constant.value.INT);
		case OrValueType::FLOAT:
			return std::to_string(constant.value.FLOAT);
		case OrValueType::BOOL:
			if (constant.value.BOOL) return "TRUE";
			return "FALSE";
		case OrValueType::NONE:
			return "None";
	}
}

void print_const(const OrValue &constant) {
	std::cout << to_string(constant);
}

void print_stack(ConstStack &stack) {
	Log::debug("Current stack:");
	std::cout << "[";
	for (OrValue constant: stack.internal) {
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
		case OP_CODE::CONST: return "CONST";
		case OP_CODE::NEGATE: return "NEGATE";
		case OP_CODE::ADD: return "ADD";
		case OP_CODE::SUBTRACT: return "SUBTRACT";
		case OP_CODE::MULTIPLY: return "MULTIPLY";
		case OP_CODE::DIVIDE: return "DIVIDE";
		case OP_CODE::NOT: return "NOT";
		case OP_CODE::INT_DIVIDE: return "INT_DIVIDE";
		case OP_CODE::EXPONENTIATION: return "EXPONENTIATION";
	}
}

} // LibOrichalcum
