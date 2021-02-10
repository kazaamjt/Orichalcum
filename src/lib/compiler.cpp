#include "compiler.hpp"

#include "chunk.hpp"
#include "debug.hpp"

namespace OrichalcumLib {

int Compiler::run() {
	Chunk chunk("test");
	chunk.write(OpCode::OP_RETURN, 1);
	chunk.write(2.3, 2);

	dissassemble_chunk(chunk);
	return 0;
}

} // namespace Orichalcum
