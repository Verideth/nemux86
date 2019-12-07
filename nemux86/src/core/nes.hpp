#pragma once

#include <cstdint>
#include <vector>
#include <stack>
#include "functional/opcodes.hpp"
#include "functional/reg_flags.hpp"

class c_nes
{
public:
	static void initialize_nes();
	static void run_cpu_clock();
	static void destroy_nes();

protected:
	static std::stack<std::uint8_t> nes_stack;
	opcode_t cur_opcode;
};

static class c_nes_cpu : c_nes
{
public:
	c_nes_cpu() = default;
	/* converts the location (addres) bytecode, also sets up cur_opcode */
	static void convert_mem_bytecode(const std::uint16_t location);
	void setup_opcode_vector();

	static opcode_t cur_opcode;
	static std::stack<std::uint16_t> cpu_stack;
	static std::uint16_t pc; // program counter, 16 bit number 
	static std::uint8_t a; // a general purpose register, 8 bit number
	static std::uint8_t sp; // stack pointer
	static std::uint8_t x; // x general purpose register
	static std::uint8_t y; // y general purpose register
	static std::uint8_t p; // p register, aka status register
	static std::uint16_t current_addressing_mode; // the current addressing mode id, defined in reg_flags.hpp
	static FLAGS cur_flag;

private:
protected:
	std::vector<opcode_t> opcode_vector;
	OPCODE_HEX hexadecimal_identification;
} nes_cpu;

namespace NES_MANIPULATION
{
	inline const std::uint16_t& get_pc() noexcept
	{
		return c_nes_cpu::pc;
	}
	inline const void set_pc(std::uint16_t new_pc) noexcept
	{
		c_nes_cpu::pc = new_pc;
	}
	inline const void inc_pc() noexcept { c_nes_cpu::pc += 2; }

	inline const std::uint8_t& get_a() noexcept
	{
		return c_nes_cpu::a;
	}
	inline const void set_a(std::uint8_t new_a) noexcept
	{
		c_nes_cpu::a = new_a;
	}

	inline const std::uint8_t& get_sp() noexcept
	{
		return c_nes_cpu::sp;
	}
	inline const void set_sp(std::uint8_t new_sp) noexcept
	{
		c_nes_cpu::sp = new_sp;
	}

	inline const std::uint16_t& get_x() noexcept
	{
		return c_nes_cpu::x;
	}
	inline const void set_x(std::uint8_t new_x) noexcept
	{
		c_nes_cpu::x = new_x;
	}

	inline const std::uint8_t& get_y() noexcept
	{
		return c_nes_cpu::y;
	}
	inline const void set_y(std::uint8_t new_y) noexcept
	{
		c_nes_cpu::y = new_y;
	}

	inline const std::uint8_t& get_p() noexcept
	{
		return c_nes_cpu::p;
	}
	inline const void set_p(std::uint8_t new_p) noexcept
	{
		c_nes_cpu::p = new_p;
	}
}
