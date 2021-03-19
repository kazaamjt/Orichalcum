#include "VM.hpp"

#include <iostream>

#include "Debug.hpp"
#include "Log.hpp"

namespace OrichalcumLib {

VM::VM():
	vm_debug(false), chunk(0), chunk_iter(chunk.get_iterator()) { }

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
				Debug::print_const(stack.pop());
				return INTERPRET_RESULT::OK;
			}

			case OP_CODE::CONST: {
				Constant constant = get_const();
				stack.push(constant);
			} break;

			case OP_CODE::SUBTRACT: {
				subtract();
			} break;
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

		// Failure
		default: break;
	}
	stack.push(constant);
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
