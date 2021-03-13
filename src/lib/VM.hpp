#pragma once
#include "Chunk.hpp"

namespace OrichalcumLib {

enum class INTERPRET_RESULT {
	OK,
	FAIL,
};

struct InterpretReport {
	INTERPRET_RESULT result;
};

class VM {
public:
	VM();
	void enable_debug();

	InterpretReport interpret(const Chunk &chunk);

private:
	bool vm_debug;

	Chunk chunk;
	ChunkIterator chunk_iter;
	Instruction instruction;

	void init_chunk(const Chunk &chunk);
	void increment();
	INTERPRET_RESULT run();
	int64_t get_const_int();
	double get_const_float();
};
} // OrichalcumLib
