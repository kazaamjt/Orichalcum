#include "Chunk.hpp"

#include "Misc.hpp"

namespace OrichalcumLib {

Chunk::Chunk(const std::string &_name): name(_name) { }
Chunk::Chunk(size_t index): name(Misc::to_hex(index)) { }

void Chunk::write(OP_CODE op_code, int line) {
	lines.push_back(line);
	Instruction instruction;
	instruction.op_code = op_code;
	instructions.push_back(instruction);
}

void Chunk::write(int64_t _constant, int line) {
	lines.push_back(line);
	Instruction instruction;
	instruction.op_code = OP_CODE::CONST_INT;
	instructions.push_back(instruction);

	Constant constant;
	constant.int_ = _constant;
	constants.push_back(constant);
	size_t index = constants.size() - 1;

	lines.push_back(line);
	Instruction const_addr;
	const_addr.index = index;
	instructions.push_back(const_addr);
}

void Chunk::write(double _constant, int line) {
	lines.push_back(line);
	Instruction instruction;
	instruction.op_code = OP_CODE::CONST_FLOAT;
	instructions.push_back(instruction);

	Constant constant;
	constant.float_ = _constant;
	constants.push_back(constant);
	size_t index = constants.size() - 1;

	lines.push_back(line);
	Instruction const_addr;
	const_addr.index = index;
	instructions.push_back(const_addr);
}

int Chunk::get_line(size_t index) {
	return lines.at(index);
}

Instruction Chunk::get(size_t index) {
	return instructions.at(index);
}

int64_t Chunk::get_int(size_t index) {
	return constants.at(index).int_;
}

double Chunk::get_float(size_t index) {
	return constants.at(index).float_;
}

size_t Chunk::size() {
	return instructions.size();
}

} // OrichalcumLib
