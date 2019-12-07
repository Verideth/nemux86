#pragma once

#include <map>
#include <functional>
#include <cstdint>
#include "opcodes.hpp"
#include "../nes.hpp"

#define OPCODERUN OPCODE_FUNCTIONALITY::opcode_execute

namespace OPCODE_FUNCTIONALITY
{
	enum class LOHI_ENUM { LOW, HIGH };
	inline std::uint8_t retrieve_byte(const std::uint16_t address, const LOHI_ENUM option)
	{
		if (option == LOHI_ENUM::LOW)
			return (address & 0x00FF);
		else
			return (address & 0xFF00);
	}
	
	inline void brk_fn()
	{
		NES_MANIPULATION::inc_pc();
		nes_cpu.cur_flag = I;
		nes_cpu.cpu_stack.push(c_nes_cpu::pc);
		nes_cpu.cpu_stack.push(c_nes_cpu::p | 0x10);
		/* finish BRK function */
	}

	inline std::uint8_t ora_fn_1(std::int16_t bytecode)
	{
		const std::uint8_t operand = retrieve_byte(bytecode, LOHI_ENUM::LOW);
		// std::uint8_t res = c_nes_cpu::a | operand;

		return operand;
	}

	template <class FN_T>
	static const std::map<OPCODE_HEX, std::function<FN_T()>> operational_functions =
	{
		{ORA_1, ora_fn_1},
	};

	inline void opcode_execute(opcode_t opcode_to_exec)
	{
		switch (opcode_to_exec.l_endian)
		{
		case ORA_1: ora_fn_1(opcode_to_exec.bytecode); break;
			/* finish up this switch statement */
		};
	}

}