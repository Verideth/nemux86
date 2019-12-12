#include "nes.hpp"
#include "../nemu_main.hpp"
#include <memory>
#include "opcodes.hpp"
#include "opcode_functionality.hpp"
#include "nes_cpu.hpp"
#include "../nemu_main.hpp"

std::vector<opcode_stru> g_opcodes_vector;
void gather_opcodes(opcode_stru& opcode_to_push)
{
	OPCODE_FUNCTIONALITY::gfn_opcode_execute(opcode_to_push);
	g_opcodes_vector.push_back(opcode_to_push);
}

void nes_cpu_stru::fn_initialize_cpu()
{
	this->fn_setup_opcode_vector();
}

void nes_cpu_stru::fn_convert_mem_bytecode(const std::uint16_t location)
{
	opcode_stru opcode;
	opcode.bytecode = (g_nemu_ptr->memory[location] << 8) | (g_nemu_ptr->memory[location + 1]); 
	opcode.l_endian = (opcode.bytecode & 0x00FF);
	opcode.b_endian = (opcode.bytecode & 0xFF00);
	opcode.isolated_b_endian = opcode.b_endian >> 4; // isolates the b endian to itself.

	gather_opcodes(opcode);
}

void nes_cpu_stru::fn_setup_opcode_vector()
{
	this->opcode_vector = g_opcodes_vector;
}