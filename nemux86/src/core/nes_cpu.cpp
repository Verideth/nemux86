#include "nes_cpu.hpp"
#include "../nemu_main.hpp"
#include <memory>

c_nes_cpu* global_nes_cpu_ptr = new c_nes_cpu();

void c_nes_cpu::convert_mem_bytecode(std::int16_t location)
{
	opcode_t opcode;
	opcode.bytecode = (nemu_ptr->memory[location] << 8) | (nemu_ptr->memory[location + 1]); 
	opcode.l_endian = (opcode.bytecode & 0x00FF);
	opcode.b_endian = (opcode.bytecode & 0xFF00);
	this->cur_opcode = opcode;

	this->gather_opcodes(this->cur_opcode.l_endian);
}

std::int32_t c_nes_cpu::gather_opcodes(std::uint16_t l_endian)
{
	if (l_endian)
	{
		switch (l_endian)
		{
		case OPCODE_HEX::BRK_1:
			std::printf("BRK\n");
			this->cur_opcode.opcode_name = "BRK";
			break;
		case OPCODE_HEX::ORA_1:
			std::printf("ORA D, X\n");
			this->cur_opcode.opcode_name = "ORA";
			break;
		case OPCODE_HEX::STP_1:
			std::printf("STP\n");
			this->cur_opcode.opcode_name = "STP";
			break;
		case OPCODE_HEX::SLO_1:
			std::printf("SLO D, X\n");
			this->cur_opcode.opcode_name = "SLO";
			break;
		case OPCODE_HEX::NOP_1:
			std::printf("NOP D\n");
			this->cur_opcode.opcode_name = "NOP";
			break;
		case OPCODE_HEX::SEI_1:
			std::printf("SEI\n");
			this->cur_opcode.opcode_name = "SEI";
			break;
		/* FINISH UP THE OPCODE SWITCH */
		}
	}

	this->opcodes_vector.push_back(this->cur_opcode);

	return 1;
}