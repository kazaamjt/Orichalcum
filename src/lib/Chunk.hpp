#pragma once
#include <string>
#include <vector>

namespace OrichalcumLib {

enum class OP_CODE {
	RETURN = 0,
	CONST = 1,

	SUBTRACT = 2,
};

union Instruction {
	OP_CODE op_code;
	size_t index;
};

enum class CONSTANT_TYPE {
	INT,
	FLOAT,
};

union ConstantValue {
	int64_t int_;
	double float_;
};

struct Constant {
	CONSTANT_TYPE type;
	ConstantValue value;
};

struct Line {
	int repeat;
	int line;

	Line(int repeat, int line);
};

using ChunkIterator = std::vector<Instruction>::iterator;

class Chunk {
public:
	Chunk(const std::string &name);
	Chunk(size_t index);

	void write(OP_CODE op_code, int line);
	void write(int64_t constant, int line);
	void write(double constant, int line);

	ChunkIterator get_iterator();
	ChunkIterator next(ChunkIterator iterator);
	Instruction get(size_t index);
	int get_line(size_t index);
	Constant get_const(size_t index);

	size_t size();
	std::string name;

private:
	friend void disassemble_chunk(Chunk &chunk);
	std::vector<Instruction> instructions;
	std::vector<Constant> constants;
	std::vector<Line> lines;

	void init();
	void write_line(int line);
};

} // OrichalcumLib
