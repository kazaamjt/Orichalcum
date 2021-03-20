#pragma once
#include <vector>

#include "Chunk.hpp"

namespace OrichalcumLib {

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

} // namespace OrichalcumLib
