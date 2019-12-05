#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <stack>
#include "functional/opcodes.hpp"

class c_nes
{
public:
	static std::int16_t pc;
	void initialize_nes();
	void run_cpu_clock();
	void destroy_nes();

protected:
	std::stack<std::uint8_t> nes_stack;
	opcode_t cur_opcode;
};

class c_nes_cpu : c_nes
{
public:
	c_nes_cpu() : cur_opcode(cur_opcode) {  }
	/* converts the location (addres) bytecode, also sets up cur_opcode */
	void convert_mem_bytecode(std::int16_t location);
	std::int32_t gather_opcodes(std::uint16_t l_endian);
	opcode_t cur_opcode;

private:


protected:
	std::vector<opcode_t> opcodes_vector;
	static std::uint8_t pc; // program counter, 8 bit number
	static std::uint8_t a; // a general purpose register, 8 bit number
	static std::uint8_t x; // x general purpose register
	static std::uint8_t y; // y general purpose register
	static std::uint8_t sp; // stack pointer
};

extern c_nes_cpu* global_nes_cpu_ptr;