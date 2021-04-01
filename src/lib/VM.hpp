#pragma once
#include "Chunk.hpp"
#include "data_structures.hpp"

namespace LibOrichalcum {

enum class INTERPRET_RESULT {
	OK,
	FAIL,
};

struct InterpretReport {
	INTERPRET_RESULT result;
};

enum class BINARY_OP {
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
};

class VM {
public:
	VM();
	void enable_debug();

	InterpretReport interpret(const Chunk &chunk);

private:
	bool vm_debug = false;

	Chunk chunk;
	ChunkIterator chunk_iter;
	Instruction instruction;
	ConstStack stack;

	void init_chunk(const Chunk &chunk);
	void increment();
	INTERPRET_RESULT run();
	Constant get_const();

	void subtract();
	void binary_op(BINARY_OP op);
	int64_t calc(BINARY_OP op, int64_t a, int64_t b);
	double calc(BINARY_OP op, double a, double b);
};
} // LibOrichalcum
