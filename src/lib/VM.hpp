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
	INT_DIVIDE,
	EXPONENTIATION,
};

std::string to_string(BINARY_OP op);
BINARY_OP to_opcode(Instruction instruction);

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

bool truthiness(OrValue or_value);

class VM {
public:
	VM();
	VM(bool debug);

	InterpretReport interpret(std::shared_ptr<Chunk> chunk);

private:
	bool debug = false;

	std::shared_ptr<Chunk> chunk;
	ConstStack stack;
	Instruction current;

	void init_chunk(std::shared_ptr<Chunk> chunk);
	INTERPRET_RESULT run();

	void negate();
	void unary_not();

	void binary_op();
	OrValue calc_1(BINARY_OP op, OrValue a, OrValue b);
	int64_t sub_calc_1(BINARY_OP op, int64_t a, int64_t b);
	OrValue calc_2(BINARY_OP op, OrValue a, OrValue b);
	double sub_calc_2(BINARY_OP op, double a, double b);
};
} // LibOrichalcum
