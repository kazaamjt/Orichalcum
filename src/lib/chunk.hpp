#pragma once
#include <vector>
#include <string>

namespace OrichalcumLib {

enum class OpCode {
	OP_RETURN,
	OP_CONSTANT
};

typedef double Value;

union ByteCode {
	OpCode op_code;
	size_t index;
};

class Chunk {
public:
	Chunk(const std::string &name);
	~Chunk();

	void write(OpCode op_code, int line);
	void write(Value constant, int line);

	friend void dissassemble_chunk(const Chunk &chunk);

private:
	std::string name;
	std::vector<ByteCode> byte_codes;
	std::vector<Value> constants;
	std::vector<int> lines;

	void add_constant(Value value);
};

} // namespace Orichalcum
