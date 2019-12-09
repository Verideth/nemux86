#pragma once

#include <cstdint>
#include <vector>
#include "nes.hpp"
#include <map>
#include "opcodes.hpp"
#include <functional>

namespace OPCODE_FUNCTIONALITY
{
	inline std::int16_t gfn_count_bytes(const std::uint16_t address)
	{
		const std::vector<char> byte_data(address);
		return static_cast<std::int16_t>(byte_data.size());
	}

	inline void gfn_setzn(const std::uint16_t address)
	{
		nes_cpu.fl_z = !address;
		nes_cpu.fl_n = address & 0x80;
	}

	enum class LOHI_E { LOW, HIGH };
	inline std::uint8_t gfn_retrieve_byte(const std::uint16_t address, const LOHI_E option)
	{
		if (option == LOHI_E::LOW)
			return (address & 0x00FF);
		else
			return ((address & 0xFF00) >> 4);
	}

	inline void gfn_brk()
	{
		NES_MANIPULATION::inc_pc();
		nes_cpu.cpu_stack.push(nes_cpu.pc);
		nes_cpu.cpu_stack.push(nes_cpu.p | 0x10);
		/* finish BRK function */
	}

	inline std::uint8_t gfn_ora1(std::int16_t bytecode)
	{
		const std::uint8_t operand = gfn_retrieve_byte(bytecode, LOHI_E::LOW);
		nes_cpu.a |= operand;

		return nes_cpu.a;
	}

	inline void gfn_nop() {  }

	template <class FN_T>
	static const std::map<OPCODE_HEX, std::function<FN_T()>> operational_functions =
	{
		{ORA_1, gfn_ora1},
	};

	inline void opcode_execute(const opcode_t& opcode_to_exec)
	{
		switch (opcode_to_exec.l_endian)
		{
		case ORA_1: gfn_ora1(opcode_to_exec.bytecode); break;
			/* get the functionality for the opcodes in between */
		case NOP_1: gfn_nop(); break;

		default:
			NES_MANIPULATION::inc_pc();
		};
	}

}
