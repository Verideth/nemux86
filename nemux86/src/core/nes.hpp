#pragma once

#include <cstdint>
#include <vector>
#include <stack>
#include "opcodes.hpp"

/* not sure if i'll implement this enum yet */
enum REGISTERS
{
	A_REG = 0x01, // byte wide accumlator
	X_REG = 0x02, // byte wide, general purpose
	Y_REG = 0x03, // byte wide, 
	PC_REG = 0x04, // program counter
	SP_REG = 0x05, // stack pointer
	P_REG = 0x06 // register used by the ALU, used in logic, arithmetic, and branch instructions.
				// known as the "status register" according to nesdev
};

enum ADDRESSING_MODES
{
	ZPI_X = 0x01, // zero page indexed d, x
	ZPI_Y = 0x02, // zero page indexed d, y
	ABSINDEX_X = 0x03, // absolute index d, x
	ABSINDEX_Y = 0x04, // absolute index d, y
	INDEXINDIR_X = 0x05, // index indirect d, x
	INDEXINDIR_Y = 0x06 // index indirect d, y
};

static struct nes_str
{
	nes_str() = default;
	void fn_initialize_nes();
	void fn_run_cpu_clock();
	void fn_destroy_nes();

protected:
	opcode_t cur_opcode;
} nes;

static struct nes_cpu_str : nes_str
{
	nes_cpu_str() = default;
	/* converts the location (address) bytecode, also sets up cur_opcode */
	void fn_convert_mem_bytecode(const std::uint16_t location);
	void fn_setup_opcode_vector();

	static opcode_t cur_opcode;
	std::stack<std::uint16_t> cpu_stack;
	std::uint16_t pc; // program counter, 16 bit number 
	std::uint8_t a; // a general purpose register, 8 bit number
	std::uint8_t sp; // stack pointer
	std::uint8_t x; // x general purpose register
	std::uint8_t y; // y general purpose register
	std::uint8_t p; // p register, aka status register
	std::uint16_t current_addressing_mode; // the current addressing mode id, defined in reg_flags.hpp

	std::int16_t fl_c, fl_z, fl_i, fl_n, fl_v, fl_b, fl_d;
	
protected:
	std::vector<opcode_t> opcode_vector;
} nes_cpu;

namespace NES_MANIPULATION
{
	inline const std::uint16_t& get_pc() noexcept
	{
		return nes_cpu.pc;
	}
	inline void gfn_set_pc(const std::uint16_t new_pc) noexcept
	{
		nes_cpu.pc = new_pc;
	}
	inline void inc_pc() noexcept
	{
		nes_cpu.pc += 2;
	}

	inline const std::uint8_t& get_a() noexcept
	{
		return nes_cpu.a;
	}
	inline void set_a(const std::uint8_t new_a) noexcept
	{
		nes_cpu.a = new_a;
	}

	inline const std::uint8_t& gfn_get_sp() noexcept
	{
		return nes_cpu.sp;
	}
	inline void gfn_set_sp(const std::uint8_t new_sp) noexcept
	{
		nes_cpu.sp = new_sp;
	}

	inline const std::uint16_t& gfn_get_x() noexcept
	{
		return nes_cpu.x;
	}
	inline void gfn_set_x(const std::uint8_t new_x) noexcept
	{
		nes_cpu.x = new_x;
	}

	inline const std::uint8_t& gfn_get_y() noexcept
	{
		return nes_cpu.y;
	}
	inline void gfn_set_y(const std::uint8_t new_y) noexcept
	{
		nes_cpu.y = new_y;
	}

	inline const std::uint8_t& get_p() noexcept
	{
		return nes_cpu.p;
	}
	inline void set_p(const std::uint8_t new_p) noexcept
	{
		nes_cpu.p = new_p;
	}
}
