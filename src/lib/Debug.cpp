#include "Debug.hpp"

#include <iostream>
#include <iomanip>

#include "Misc.hpp"
#include "Log.hpp"

namespace OrichalcumLib::Debug {

static void print_line(int line) {
	static int last_line = 0;
	std::cout << "line ";
	if (last_line != line) {
		std::cout << std::left << std::setfill(' ') << std::setw(5) << line;
	}
	else std::cout << "|    ";
	last_line = line;
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
	std::cout << Misc::to_hex(index);
	print_line(chunk.get_line(index));
	switch (chunk.get(index).op_code) {
		case OP_CODE::RETURN: {
			std::cout << " RETURN" << std::endl;
		} break;

		case OP_CODE::CONST: {
			std::cout << " CONST_INT" << std::endl;
			size_t const_index = chunk.get(++index).index;
			print_line(chunk.get_line(index));
			std::cout << Misc::to_hex(index) << " CONST_INDEX => ";
			std::cout << Misc::to_hex(const_index) << " = ";
			print_const(chunk.get_const(const_index));
			std::cout << std::endl;
		} break;

		case OP_CODE::SUBTRACT: {
			std::cout << "SUBTRACT" << std::endl;
		}
	}

	return ++index;
}

void print_const(const Constant &constant) {
	switch (constant.type) {
		case CONSTANT_TYPE::INT: {
			std::cout << constant.value.int_;
		} break;

		case CONSTANT_TYPE::FLOAT: {
			std::cout << constant.value.float_;
		} break;
	}
}

template <typename T>
void print_stack(Stack<T> stack) {
	Log::debug("Current stack:");
	std::cout << "[";
	for (T constant: stack.internal) {
		print_const(constant);
		if (constant != stack.internal.end()) {
			std::cout << " ";
		}
	}
	std::cout << "]" << std::endl;
}

} // OrichalcumLib
