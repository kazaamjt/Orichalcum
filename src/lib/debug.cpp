#include "debug.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>


namespace OrichalcumLib {

void dissassemble_chunk(const Chunk &chunk) {
	std::cout << "====== Chunk " << chunk.name << " ======" << std::endl;

	for (size_t i = 0; i < chunk.byte_codes.size(); i++) {
		std::stringstream number;
		number << std::setw(4) << std::setfill('0') << std::hex << i;

		std::stringstream line_number;
		if (i == 0 || chunk.lines[i] > chunk.lines[i - 1]) {
			line_number << std::setw(10) << std::setfill(' ') << std::left;
			line_number << "line " + std::to_string(chunk.lines[i]);
		} else {
			line_number << std::setw(10) << std::setfill(' ') << std::left << " |";
		}

		OpCode op_code = chunk.byte_codes[i].op_code;
		std::cout << number.str() << "  " << line_number.str();
		std::cout << " " << opcode_to_string(op_code) << std::endl;

		if (op_code == OpCode::OP_CONSTANT) {
			std::stringstream const_number;
			const_number << std::setw(4) << std::setfill('0') << std::hex << ++i;

			std::stringstream const_line_number;
			const_line_number << std::setw(10) << std::setfill(' ') << std::left << " |";

			size_t const_index = chunk.byte_codes[i].index;
			std::cout << const_number.str() << "  " << const_line_number.str() << " CONST_INDEX ";
			std::cout << const_index << " => " << chunk.constants[const_index] << std::endl;
		}
	}
}

std::string opcode_to_string(OpCode op_code) {
	switch(op_code) {
		case OpCode::OP_RETURN: return "OP_RETURN";
		case OpCode::OP_CONSTANT: return "OP_CONSTANT";
	}
}

} // namespace Orichalcum
