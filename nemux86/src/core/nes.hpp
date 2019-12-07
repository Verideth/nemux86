#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <stack>
#include "functional/opcodes.hpp"

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

class c_nes_cpu : c_nes
{
public:
	c_nes_cpu() : cur_opcode(cur_opcode), hexadecimal_identification(OPCODE_HEX::BRK_1) {  }
	/* converts the location (addres) bytecode, also sets up cur_opcode */
	void convert_mem_bytecode(const std::uint16_t location);
	std::int32_t gather_opcodes(const std::uint8_t l_endian, const std::uint16_t bytecode);
	opcode_t cur_opcode;
	static std::stack<std::uint16_t> cpu_stack;
	static std::uint8_t pc; // program counter, 8 bit number
	static std::uint8_t a; // a general purpose register, 8 bit number
	static std::uint8_t x; // x general purpose register
	static std::uint8_t y; // y general purpose register
	static std::uint8_t sp; // stack pointer

private:
protected:
	OPCODE_HEX hexadecimal_identification;
	std::vector<opcode_t> opcodes_vector;
};

extern c_nes_cpu* nes_cpu_ptr;