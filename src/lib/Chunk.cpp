#include "Chunk.hpp"

#include "Log.hpp"
#include "Misc.hpp"

namespace OrichalcumLib {

Line::Line(int _repeat, int _line): repeat(_repeat), line(_line) { }

Chunk::Chunk(const std::string &_name): name(_name) {
	init();
}

Chunk::Chunk(size_t index): name(Misc::to_hex(index)) {
	init();
}

void Chunk::init() {
	Log::debug("Initializing chunk " + name);
	lines.push_back(Line(0, 0));
}

void Chunk::write(OP_CODE op_code, int line) {
	write_line(line);
	Instruction instruction;
	instruction.op_code = op_code;
	instructions.push_back(instruction);
}

void Chunk::write(int64_t _constant, int line) {
	write_line(line);
	Instruction instruction;
	instruction.op_code = OP_CODE::CONST;
	instructions.push_back(instruction);

	Constant constant;
	constant.type = CONSTANT_TYPE::INT;
	constant.value.int_ = _constant;
	constants.push_back(constant);
	size_t index = constants.size() - 1;

	write_line(line);
	Instruction const_addr;
	const_addr.index = index;
	instructions.push_back(const_addr);
}

void Chunk::write(double _constant, int line) {
	write_line(line);
	Instruction instruction;
	instruction.op_code = OP_CODE::CONST;
	instructions.push_back(instruction);

	Constant constant;
	constant.type = CONSTANT_TYPE::FLOAT;
	constant.value.float_ = _constant;
	constants.push_back(constant);
	size_t index = constants.size() - 1;

	write_line(line);
	Instruction const_addr;
	const_addr.index = index;
	instructions.push_back(const_addr);
}

void Chunk::write_line(int line) {
	Line last_line = lines.back();

	if (last_line.line == line) {
		last_line.repeat++;
		lines.pop_back();
		lines.push_back(last_line);
	}
	else {
		Line new_line(1, line);
		lines.push_back(new_line);
	}
}

ChunkIterator Chunk::get_iterator() {
	return instructions.begin();
}

int Chunk::get_line(size_t index) {
	size_t current_index = 0;
	for (Line line: lines) {
		if (line.repeat > 0) {
			current_index += static_cast<size_t>(line.repeat);
			if (index + 1 <= current_index) {
				return line.line;
			}
		}
	}

	// This shouldn't ever happen, right?
	return lines.back().line + 1;
}

ChunkIterator Chunk::next(ChunkIterator iterator) {
	return std::next(iterator);
}

Instruction Chunk::get(size_t index) {
	return instructions.at(index);
}

Constant Chunk::get_const(size_t index) {
	return constants.at(index);
}

size_t Chunk::size() {
	return instructions.size();
}

} // OrichalcumLib
