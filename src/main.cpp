#include "lib\Chunk.hpp"
#include "lib\Debug.hpp"

int main(int argc, char *argv[]) {
	OrichalcumLib::Chunk chunk("test");
	chunk.write(OrichalcumLib::OP_CODE::RETURN, 1);
	chunk.write(static_cast<double>(0.001f), 2);
	chunk.write(static_cast<int64_t>(2021), 3);
	OrichalcumLib::Debug::disassemble_chunk(chunk);

	return 0;
}
