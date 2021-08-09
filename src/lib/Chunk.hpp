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
	NOT,
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
	INT_DIVIDE,
	EXPONENTIATION,
};

class Instruction {
public:
	Instruction(OP_CODE op_code, std::shared_ptr<Token> token);
	Instruction(OP_CODE op_code, size_t index);
	Instruction(OP_CODE op_code);

	OP_CODE op_code;
	size_t index;
	std::shared_ptr<Token> token;
	bool has_token;
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

	void write(OP_CODE op_code);
	void write(OP_CODE op_code, std::shared_ptr<Token> token);
	void write(int64_t value, std::shared_ptr<Token> token);
	void write(double value, std::shared_ptr<Token> token);
	void write(bool value, std::shared_ptr<Token> token);
	void write(OrNone value, std::shared_ptr<Token> token);

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

	void init();
};

} // LibOrichalcum
