#pragma once
#include <vector>

namespace OrichalcumLib {

template <typename T>
class Stack {
public:
	void push(const T &value) {
		internal.push_back(value);
	}

	T pop() {
		T value = internal.back();
		internal.pop_back();
		return value;
	}

private:
	friend void print_stack(Stack<T> stack);
	std::vector<T> internal;

};

} // namespace OrichalcumLib
