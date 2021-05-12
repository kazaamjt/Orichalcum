#include "Chunk.hpp"

#include "Log.hpp"
#include "Misc.hpp"

namespace LibOrichalcum {

Line::Line(int _repeat, int _line): repeat(_repeat), line(_line) { }

Chunk::Chunk(const std::string &_name): name(_name) {
	init();
}

Chunk::Chunk(size_t index): name(Misc::to_hex(index)) {
	init();
}

void Chunk::init() {
	Log::debug("Initializing chunk " + name);
}

void Chunk::write(std::shared_ptr<Token> token) {
	tokens.push_back(token);
}

void Chunk::write(OP_CODE op_code, std::shared_ptr<Token> token) {
	write(token);
	Instruction instruction;
	instruction.op_code = op_code;
	instructions.push_back(instruction);
}

void Chunk::write(int64_t _constant, std::shared_ptr<Token> token) {
	write(token);
	Instruction instruction;
	instruction.op_code = OP_CODE::CONST_;
	instructions.push_back(instruction);

	Constant constant;
	constant.type = CONSTANT_TYPE::INT;
	constant.value.int_ = _constant;
	constants.push_back(constant);
	size_t index = constants.size() - 1;

	write(token);
	Instruction const_addr;
	const_addr.index = index;
	instructions.push_back(const_addr);
}

void Chunk::write(double _constant, std::shared_ptr<Token> token) {
	write(token);
	Instruction instruction;
	instruction.op_code = OP_CODE::CONST_;
	instructions.push_back(instruction);

	Constant constant;
	constant.type = CONSTANT_TYPE::FLOAT;
	constant.value.float_ = _constant;
	constants.push_back(constant);
	size_t index = constants.size() - 1;

	write(token);
	Instruction const_addr;
	const_addr.index = index;
	instructions.push_back(const_addr);
}

ChunkIterator Chunk::get_iterator() {
	return instructions.begin();
}

std::shared_ptr<Token> Chunk::get_token(size_t index) {
	return tokens.at(index);
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

} // LibOrichalcum
