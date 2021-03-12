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
	else std::cout << "|     ";
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
	print_line(chunk.get_line(index));
	switch (chunk.get(index).op_code) {
		case OP_CODE::RETURN: {
			std::cout << Misc::to_hex(index) << " RETURN" << std::endl;
		} break;

		case OP_CODE::CONST_INT: {
			std::cout << Misc::to_hex(index) << " CONST_INT" << std::endl;
			size_t const_index = chunk.get(++index).index;
			print_line(chunk.get_line(index));
			std::cout << Misc::to_hex(index) << " CONST_INDEX ";
			std::cout << Misc::to_hex(const_index) << " [" << chunk.get_int(const_index) << "]" << std::endl;
		} break;

		case OP_CODE::CONST_FLOAT: {
			std::cout << Misc::to_hex(index) << " CONST_FLOAT" << std::endl;
			size_t const_index = chunk.get(++index).index;
			print_line(chunk.get_line(index));
			std::cout << Misc::to_hex(index) << " CONST_INDEX ";
			std::cout << Misc::to_hex(const_index) << " [" << chunk.get_float(const_index) << "]" << std::endl;
		} break;
	}

	return ++index;
}

} // OrichalcumLib
