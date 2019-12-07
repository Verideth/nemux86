#pragma once

#include <map>
#include <functional>
#include <cstdint>
#include "opcodes.hpp"
#include "../nes.hpp"

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
		c_nes_cpu::cpu_stack.push(c_nes_cpu::pc);
		/* finish BRK function */
	}

	inline std::uint8_t ora_fn_1(std::int16_t bytecode)
	{
		std::uint8_t operand = retrieve_byte(bytecode, LOHI_ENUM::LOW);
		std::uint8_t res = c_nes_cpu::a | operand;

		return operand;
	}

	inline void opcode_execute(OPCODE_HEX opcode)
	{
		switch (opcode)
		{
		case OPCODE_HEX::ORA_1:
				
			break;
			/* finish up this switch statement */
		};
	}

	static const std::map<OPCODE_HEX, std::function<void()>> operational_functions =
	{ {OPCODE_HEX::BRK_1, brk_fn} };
	
}