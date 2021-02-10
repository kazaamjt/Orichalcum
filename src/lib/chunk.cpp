#include "chunk.hpp"

namespace OrichalcumLib {

Chunk::Chunk(const std::string &_name): name(_name) { }
Chunk::~Chunk() { }

void Chunk::write(OpCode op_code, int line) {
	ByteCode byte_code = { };
	byte_code.op_code = op_code;
	byte_codes.push_back(byte_code);
	lines.push_back(line);
}

void Chunk::write(Value constant, int line) {
	ByteCode byte_code = { };
	byte_code.index = constants.size();
	constants.push_back(constant);

	write(OpCode::OP_CONSTANT, line);
	byte_codes.push_back(byte_code);
	lines.push_back(line);
}

void Chunk::add_constant(Value value) {
	constants.push_back(value);
}

} // namespace Orichalcum

