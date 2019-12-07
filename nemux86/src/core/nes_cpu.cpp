#include "nes.hpp"
#include "../nemu_main.hpp"
#include <memory>

c_nes_cpu* nes_cpu_ptr = new c_nes_cpu();

void c_nes_cpu::convert_mem_bytecode(const std::uint16_t location)
{
	opcode_t opcode;
	opcode.bytecode = (nemu_ptr->memory[location] << 8) | (nemu_ptr->memory[location + 1]); 
	opcode.l_endian = (opcode.bytecode & 0x00FF);
	opcode.b_endian = (opcode.bytecode & 0xFF00);
	
	this->cur_opcode = opcode;
	this->gather_opcodes(this->cur_opcode.l_endian, this->cur_opcode.bytecode);
}

std::int32_t c_nes_cpu::gather_opcodes(const std::uint8_t l_endian, const std::uint16_t bytecode)
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
		case OPCODE_HEX::ORA_2:
			std::printf("ORA D\n");
			this->cur_opcode.opcode_name = "ORA";
			break;
		case OPCODE_HEX::ASL_1:
			std::printf("ASL D\n");
			this->cur_opcode.opcode_name = "ASL";
			break;
		case OPCODE_HEX::SLO_2:
			std::printf("SLO D\n");
			this->cur_opcode.opcode_name = "SLO";
			break;
		case OPCODE_HEX::PHP_1:
			std::printf("PHP\n");
			this->cur_opcode.opcode_name = "PHP";
			break;
		case OPCODE_HEX::ORA_3:
			std::printf("ORA #I\n");
			this->cur_opcode.opcode_name = "ORA";
			break;
		case OPCODE_HEX::ASL_2:
			std::printf("ASL A\n");
			this->cur_opcode.opcode_name = "ASL";
			break;	
		case OPCODE_HEX::ANC_1:
			std::printf("ANC #I\n");
			this->cur_opcode.opcode_name = "ANC";
			break;
		case OPCODE_HEX::NOP_2:
			std::printf("NOP A\n");
			this->cur_opcode.opcode_name = "NOP";
			break;
		case OPCODE_HEX::ORA_4:
			std::printf("ORA A\n");
			this->cur_opcode.opcode_name = "ORA";
		case OPCODE_HEX::ASL_3:
			std::printf("ASL A\n");
			this->cur_opcode.opcode_name = "ASL";
			break;
		case OPCODE_HEX::SLO_3:
			std::printf("SLO A\n");
			this->cur_opcode.opcode_name = "SLO";
			break;
		case OPCODE_HEX::BPL_1:
			std::printf("BPL (PC + D)\n");
			this->cur_opcode.opcode_name = "BPL";
			break;
		case OPCODE_HEX::ORA_5:
			std::printf("ORA D, Y\n");
			this->cur_opcode.opcode_name = "ORA";
			break;
		case OPCODE_HEX::STP_2:
			std::printf("STP\n");
			this->cur_opcode.opcode_name = "STP";
			break;
		case OPCODE_HEX::SLO_4:
			std::printf("SLO D, Y\n");
			this->cur_opcode.opcode_name = "SLO";
			break;
		case OPCODE_HEX::NOP_3:
			std::printf("NOP\n");
			this->cur_opcode.opcode_name = "NOP";
			break;
		case OPCODE_HEX::ORA_6:
			std::printf("ORA D, X\n");
			this->cur_opcode.opcode_name = "ORA";
			break;
		case OPCODE_HEX::ASL_4:
			std::printf("ASL D, X\n");
			this->cur_opcode.opcode_name = "ASL";
			break;
		case OPCODE_HEX::SLO_5:
			std::printf("SLO D, X\n");
			this->cur_opcode.opcode_name = "SLO";
			break;
		case OPCODE_HEX::CLC_1:
			std::printf("CLC\n");
			this->cur_opcode.opcode_name = "CLC";
			break;
		case OPCODE_HEX::ORA_7:
			std::printf("ORA A, Y\n");
			this->cur_opcode.opcode_name = "ORA";	
			break;
		case OPCODE_HEX::NOP_4:
			std::printf("NOP\n");
			this->cur_opcode.opcode_name = "NOP";
			break;
		/* FINISH UP THE OPCODE SWITCH */
		case OPCODE_HEX::EOR_2:
			std::printf("EOR D, X\n");
			this->cur_opcode.opcode_name = "EOR";
			break;
		default:
			std::printf("ERROR: UNK OPCODE\n");
			break;
		}
	}

	this->opcodes_vector.push_back(this->cur_opcode);

	return 1;
}