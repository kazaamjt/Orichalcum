#pragma once
#include <memory>

#include "Chunk.hpp"

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

struct ConstStack {
	void push(const Constant &value) {
		internal.push_back(value);
	}

	Constant pop() {
		Constant value = internal.back();
		internal.pop_back();
		return value;
	}

	std::vector<Constant> internal;
};

class VM {
public:
	VM();
	VM(bool debug);

	InterpretReport interpret(std::shared_ptr<Chunk> chunk);

private:
	bool debug = false;

	std::shared_ptr<Chunk> chunk;
	ChunkIterator chunk_iter;
	Instruction instruction;
	ConstStack stack;

	void init_chunk(std::shared_ptr<Chunk> chunk);
	void increment();
	INTERPRET_RESULT run();
	Constant get_const();

	void subtract();
	void binary_op(BINARY_OP op);
	int64_t calc(BINARY_OP op, int64_t a, int64_t b);
	double calc(BINARY_OP op, double a, double b);
};
} // LibOrichalcum
