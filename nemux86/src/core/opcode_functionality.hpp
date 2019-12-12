#pragma once

#include <cstdint>
#include <vector>
#include "nes.hpp"
#include <map>
#include "opcodes.hpp"
#include <functional>
#include "nes_cpu.hpp"

constexpr std::int16_t ADDRESS_MODE_MASK = 0x1C;;
constexpr std::int16_t ADDRESS_MODE_SHIFT = 2;

enum ADDRESS_MODES_INDEXED_E
{
	ZERO_PAGE_INDEX_X,
	ZERO_PAGE_INDEX_Y,
	ABS_INDEX_X,
	ABS_INDEX_Y,
	INDEX_INDIR_X,
	INDEX_INDIR_Y
};

enum ADDRESS_MODES_NOINDEX_E
{
	IMMEDIATE,
	
};

namespace OPCODE_FUNCTIONALITY
{
	inline void gfn_setzn(const std::uint16_t address)
	{
		g_nes_cpu.registers.fl_z = !address;
		g_nes_cpu.registers.fl_n = address & 0x80;
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
		NES_MANIPULATION::gfn_inc_pc();
		g_nes_cpu.cpu_stack.push(g_nes_cpu.registers.pc);
		g_nes_cpu.cpu_stack.push(g_nes_cpu.registers.p | 0x10);
		/* finish BRK function */
	}

	inline std::uint8_t gfn_ora(std::int16_t bytecode)
	{
		return g_nes_cpu.registers.a;
	}

	inline void gfn_nop()
	{
		NES_MANIPULATION::gfn_inc_pc(); 
	}

	template <class FN_T>
	static const std::map<OPCODE_HEX, std::function<FN_T()>> operational_functions =
	{
		{ORA_1, gfn_ora},
	};

	inline void gfn_opcode_execute(const opcode_stru& opcode_to_exec)
	{
		const std::uint8_t address_mode = ((opcode_to_exec.bytecode & ADDRESS_MODE_MASK) >> ADDRESS_MODE_MASK);
		g_nes_cpu.registers.current_addressing_mode = address_mode;


		/*
		 * implement later
		switch (address_mode)
		{
			
		}
		*/
		
		switch (opcode_to_exec.l_endian)
		{
			/* finish opcode switch statement. will be pretty massive */
		case ORA_1: gfn_ora(opcode_to_exec.bytecode); break;
		case NOP_1: gfn_nop(); break;

		default:
			NES_MANIPULATION::gfn_inc_pc();
		};
	}

}
