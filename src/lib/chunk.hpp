#pragma once
#include <vector>

enum class OpCode {
	OP_RETURN
};

struct Chunk {
	Chunk(/* args */);
	~Chunk();

	std::vector<OpCode> OpCodes;

};
