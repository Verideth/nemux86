#pragma once

#include <cstdint>
#include "opcodes.hpp"
#include "nes.hpp"
#include <stack>
#include <vector>

/* implement later */
enum ADDRESSING_MODES_E
{
	ZPI_X = 0x01, // zero page indexed d, x
	ZPI_Y = 0x02, // zero page indexed d, y
	ABSINDEX_X = 0x03, // absolute index d, x
	ABSINDEX_Y = 0x04, // absolute index d, y
	INDEXINDIR_X = 0x05, // index indirect d, x
	INDEXINDIR_Y = 0x06 // index indirect d, y
};

struct cpu_registers_stru
{
	std::uint16_t pc = 0; // program counter, 16 bit number 
	std::uint8_t a = 0; // a general purpose register, 8 bit number
	std::uint8_t sp = 0; // stack pointer
	std::uint8_t x = 0; // x general purpose register
	std::uint8_t y = 0; // y general purpose register
	std::uint8_t p = 0; // p register, aka status register
	std::uint16_t current_addressing_mode = 0; // the current addressing mode

	std::int16_t fl_c = 0, fl_z = 0, fl_i = 0, fl_n = 0, fl_v = 0, fl_b = 0, fl_d = 0;
};

static struct nes_cpu_stru : nes_stru
{
	nes_cpu_stru() = default;
	/* converts the location (address) bytecode, also sets up cur_opcode */
	void fn_initialize_cpu();
	void fn_convert_mem_bytecode(const std::uint16_t location);
	void fn_setup_opcode_vector();

	static opcode_stru cur_opcode;
	std::stack<std::uint16_t> cpu_stack;
	cpu_registers_stru registers;

protected:
	std::vector<opcode_stru> opcode_vector;
} g_nes_cpu;

namespace NES_MANIPULATION
{
	inline const std::uint16_t& gfn_get_pc() noexcept
	{
		return g_nes_cpu.registers.pc;
	}
	inline void gfn_set_pc(const std::uint16_t new_pc) noexcept
	{
		g_nes_cpu.registers.pc = new_pc;
	}
	inline void gfn_inc_pc() noexcept
	{
		g_nes_cpu.registers.pc += 2;
	}

	inline const std::uint8_t& gfn_get_a() noexcept
	{
		return g_nes_cpu.registers.a;
	}
	inline void gfn_set_a(const std::uint8_t new_a) noexcept
	{
		g_nes_cpu.registers.a = new_a;
	}

	inline const std::uint8_t& gfn_get_sp() noexcept
	{
		return g_nes_cpu.registers.sp;
	}
	inline void gfn_set_sp(const std::uint8_t new_sp) noexcept
	{
		g_nes_cpu.registers.sp = new_sp;
	}

	inline const std::uint16_t& gfn_get_x() noexcept
	{
		return g_nes_cpu.registers.x;
	}
	inline void gfn_set_x(const std::uint8_t new_x) noexcept
	{
		g_nes_cpu.registers.x = new_x;
	}

	inline const std::uint8_t& gfn_get_y() noexcept
	{
		return g_nes_cpu.registers.y;
	}
	inline void gfn_set_y(const std::uint8_t new_y) noexcept
	{
		g_nes_cpu.registers.y = new_y;
	}

	inline const std::uint8_t& gfn_get_p() noexcept
	{
		return g_nes_cpu.registers.p;
	}
	inline void gfn_set_p(const std::uint8_t new_p) noexcept
	{
		g_nes_cpu.registers.p = new_p;
	}
}
