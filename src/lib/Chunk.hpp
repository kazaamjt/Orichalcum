#pragma once
#include <string>
#include <vector>

#include "OrObject.hpp"
#include "Token.hpp"

namespace LibOrichalcum {

enum class OP_CODE {
	RETURN,
	CONST,

	NEGATE,
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
};

union Instruction {
	OP_CODE op_code;
	size_t index;
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

	void write(OP_CODE op_code, std::shared_ptr<Token> token);
	void write(int64_t constant, std::shared_ptr<Token> token);
	void write(double constant, std::shared_ptr<Token> token);

	ChunkIterator get_iterator();
	ChunkIterator next(ChunkIterator iterator);
	Instruction get(size_t index);
	std::shared_ptr<Token> get_token(size_t index);
	OrValue get_const(size_t index);

	size_t size();
	std::string name;

private:
	friend void disassemble_chunk(Chunk &chunk);
	std::vector<Instruction> instructions;
	std::vector<OrValue> constants;
	std::vector<std::shared_ptr<Token>> tokens;

	void init();
	void write(std::shared_ptr<Token> token);
};

} // LibOrichalcum
