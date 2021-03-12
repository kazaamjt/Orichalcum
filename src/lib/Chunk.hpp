#pragma once
#include <string>
#include <vector>

namespace OrichalcumLib {

enum class OP_CODE {
	RETURN = 0,
	CONST_INT = 1,
	CONST_FLOAT = 2,
};

union Instruction {
	OP_CODE op_code;
	size_t index;
};

union Constant{
	int64_t int_;
	double float_;
};

class Chunk {
public:
	Chunk(const std::string &name);
	Chunk(size_t index);

	void write(OP_CODE op_code, int line);
	void write(int64_t constant, int line);
	void write(double constant, int line);

	int get_line(size_t index);
	Instruction get(size_t index);
	int64_t get_int(size_t index);
	double get_float(size_t index);

	size_t size();

	std::string name;

private:
	friend void disassemble_chunk(Chunk &chunk);
	std::vector<Instruction> instructions;
	std::vector<Constant> constants;
	std::vector<int> lines;

};

} // OrichalcumLib
