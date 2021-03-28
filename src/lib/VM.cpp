#include "VM.hpp"

#include <iostream>

#include "Debug.hpp"
#include "Log.hpp"

namespace OrichalcumLib {

VM::VM():
	chunk(0), chunk_iter(chunk.get_iterator()) { }

void VM::enable_debug() {
	vm_debug = true;
}

InterpretReport VM::interpret(const Chunk &_chunk) {
	InterpretReport interpret_report;
	init_chunk(_chunk);

	interpret_report.result = run();

	return interpret_report;
}

void VM::init_chunk(const Chunk &_chunk) {
	chunk = _chunk;
	chunk_iter = chunk.get_iterator();
}

INTERPRET_RESULT VM::run() {
	Log::debug("Vm is executing chunk " + chunk.name);
	if (vm_debug) {
		Debug::print_stack(stack);
		Debug::disassemble_chunk(chunk);
	}

	while(true) {
		increment();
		switch(instruction.op_code) {
			case OP_CODE::RETURN: {
				std::cout << "CHUNK RETURNED ";
				Debug::print_const(stack.pop());
				std::cout << std::endl;
				return INTERPRET_RESULT::OK;
			}

			case OP_CODE::CONST: {
				Constant constant = get_const();
				stack.push(constant);
			} break;

			case OP_CODE::NEGATE: {
				subtract();
			} break;
			case OP_CODE::ADD: binary_op(BINARY_OP::ADD);break;
			case OP_CODE::SUBTRACT: binary_op(BINARY_OP::SUBTRACT);break;
			case OP_CODE::MULTIPLY: binary_op(BINARY_OP::MULTIPLY);break;
			case OP_CODE::DIVIDE: binary_op(BINARY_OP::DIVIDE);break;
		}
	}
}

void VM::subtract() {
	Constant constant = stack.pop();
	switch (constant.type) {
		case CONSTANT_TYPE::INT: {
			constant.value.int_ = -constant.value.int_;
		} break;

		case CONSTANT_TYPE::FLOAT: {
			constant.value.float_ = -constant.value.float_;
		} break;
	}
	stack.push(constant);
}

void VM::binary_op(BINARY_OP op) {
	// b first, this is important
	Constant const_b = stack.pop();
	Constant const_a = stack.pop();
	Constant const_c;
	switch (const_a.type) {
		case CONSTANT_TYPE::INT: {
			switch (const_b.type) {
				case CONSTANT_TYPE::INT: {
					const_c.type = CONSTANT_TYPE::INT;
					const_c.value.int_ = calc(op, const_a.value.int_, const_b.value.int_);
				} break;
				case CONSTANT_TYPE::FLOAT: {
					// warning
					const_c.type = CONSTANT_TYPE::FLOAT;
					const_c.value.float_ = calc(op, static_cast<double>(const_a.value.int_), const_b.value.float_);
				} break;
			}
		} break;
		case CONSTANT_TYPE::FLOAT: {
			switch (const_b.type) {
				case CONSTANT_TYPE::INT: {
					// warning
					const_c.type = CONSTANT_TYPE::FLOAT;
					const_c.value.float_ = calc(op, const_a.value.float_, static_cast<double>(const_b.value.int_));
				} break;
				case CONSTANT_TYPE::FLOAT: {
					const_c.type = CONSTANT_TYPE::FLOAT;
					const_c.value.float_ = calc(op, const_a.value.float_, const_b.value.float_);
				} break;
			}
		} break;
	}
	stack.push(const_c);
}

int64_t VM::calc(BINARY_OP op, int64_t a, int64_t b) {
	switch (op) {
		case BINARY_OP::ADD: return a + b;
		case BINARY_OP::SUBTRACT: return a - b;
		case BINARY_OP::MULTIPLY: return a * b;
		case BINARY_OP::DIVIDE: return a / b;
	}
}

double VM::calc(BINARY_OP op, double a, double b) {
	switch (op) {
		case BINARY_OP::ADD: return a + b;
		case BINARY_OP::SUBTRACT: return a - b;
		case BINARY_OP::MULTIPLY: return a * b;
		case BINARY_OP::DIVIDE: return a / b;
	}
}


Constant VM::get_const() {
	increment();
	return chunk.get_const(instruction.index);
}

// Gets the currents instruction, then increments the iterator
void VM::increment() {
	instruction = *chunk_iter;
	chunk_iter = chunk.next(chunk_iter);
}

} // OrichalcumLib
