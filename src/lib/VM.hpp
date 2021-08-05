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

std::string to_string(BINARY_OP op);

struct ConstStack {
	void push(const OrValue &value) {
		internal.push_back(value);
	}

	OrValue pop() {
		OrValue value = internal.back();
		internal.pop_back();
		return value;
	}

	std::vector<OrValue> internal;
};

class VM {
public:
	VM();
	VM(bool debug);

	InterpretReport interpret(std::shared_ptr<Chunk> chunk);

private:
	bool debug = false;

	std::shared_ptr<Chunk> chunk;
	ConstStack stack;

	void init_chunk(std::shared_ptr<Chunk> chunk);
	INTERPRET_RESULT run();
	OrValue get_const();

	void negate();
	void binary_op(BINARY_OP op);
	int64_t calc(BINARY_OP op, int64_t a, int64_t b);
	double calc(BINARY_OP op, double a, double b);
};
} // LibOrichalcum
