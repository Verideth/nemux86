#include "nes.hpp"
#include "../nemu_main.hpp"
#include "functional/opcodes_functional.hpp"
#include <memory>
#include <utility>

void gather_opcodes(const std::uint8_t l_endian, const std::uint16_t bytecode, opcode_t& opcode_to_push)
{
	if (l_endian)
	{
		OPCODE_FUNCTIONALITY::opcode_execute(l_endian, bytecode);
	}

	c_nes_cpu::opcodes_vector.push_back(opcode_to_push);
	return 1;
}

void c_nes_cpu::convert_mem_bytecode(const std::uint16_t location)
{
	opcode_t opcode;
	opcode.bytecode = (nemu_ptr->memory[location] << 8) | (nemu_ptr->memory[location + 1]); 
	opcode.l_endian = (opcode.bytecode & 0x00FF);
	opcode.b_endian = (opcode.bytecode & 0xFF00);

	gather_opcodes(opcode.l_endian, opcode.bytecode, opcode);
	std::exchange(opcode, 0);
}