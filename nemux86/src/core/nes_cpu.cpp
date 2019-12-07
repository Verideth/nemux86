#include "nes.hpp"
#include "../nemu_main.hpp"
#include "functional/opcodes_functional.hpp"
#include <memory>
#include <utility>

std::vector<opcode_t> opcodes_vector;
void gather_opcodes(opcode_t& opcode_to_push)
{
	OPCODERUN(opcode_to_push);
	opcodes_vector.push_back(opcode_to_push);
}

void c_nes_cpu::convert_mem_bytecode(const std::uint16_t location)
{
	opcode_t opcode;
	opcode.bytecode = (nemu_ptr->memory[location] << 8) | (nemu_ptr->memory[location + 1]); 
	opcode.l_endian = (opcode.bytecode & 0x00FF);
	opcode.b_endian = (opcode.bytecode & 0xFF00);
	opcode.isolated_b_endian = opcode.b_endian >> 4; // isolates the b endian to itself.

	gather_opcodes(opcode);
}

void c_nes_cpu::setup_opcode_vector()
{
	this->opcode_vector = opcodes_vector;
}