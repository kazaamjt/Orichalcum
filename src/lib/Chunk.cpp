#include "Chunk.hpp"

#include "Log.hpp"
#include "Misc.hpp"

namespace LibOrichalcum {

Instruction::Instruction(OP_CODE _op_code, std::shared_ptr<Token> _token):
op_code(_op_code), token(_token), has_token(true) { }

Instruction::Instruction(OP_CODE _op_code, size_t _index):
op_code(_op_code), index(_index), has_token(false) { }

Instruction::Instruction(OP_CODE _op_code):
op_code(_op_code), has_token(false) { }

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

void Chunk::write(OP_CODE op_code) {
	instructions.push_back(Instruction(op_code));
}

void Chunk::write(OP_CODE op_code, std::shared_ptr<Token> token) {
	instructions.push_back(Instruction(op_code, token));
}

void Chunk::write(int64_t value, std::shared_ptr<Token> token) {
	OrValue constant(value, token);
	constants.push_back(constant);
	size_t index = constants.size() - 1;

	instructions.push_back(Instruction(OP_CODE::CONST, index));
}

void Chunk::write(double value, std::shared_ptr<Token> token) {
	OrValue constant(value, token);
	constants.push_back(constant);
	size_t index = constants.size() - 1;

	instructions.push_back(Instruction(OP_CODE::CONST, index));
}

void Chunk::write(bool value, std::shared_ptr<Token> token) {
	OrValue constant(value, token);
	constants.push_back(constant);
	size_t index = constants.size() - 1;

	instructions.push_back(Instruction(OP_CODE::CONST, index));
}

void Chunk::write(OrNone value, std::shared_ptr<Token> token) {
	OrValue constant(value, token);
	constants.push_back(constant);
	size_t index = constants.size() - 1;

	instructions.push_back(Instruction(OP_CODE::CONST, index));
}

ChunkIterator Chunk::get_iterator() {
	return instructions.begin();
}

ChunkIterator Chunk::next(ChunkIterator iterator) {
	return std::next(iterator);
}

Instruction Chunk::get(size_t index) {
	return instructions.at(index);
}

OrValue Chunk::get_const(size_t index) {
	return constants.at(index);
}

size_t Chunk::size() {
	return instructions.size();
}

} // LibOrichalcum
