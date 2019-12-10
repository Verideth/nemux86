#pragma once

#include "opcodes.hpp"

/* not sure if i'll implement this enum yet */
enum REGISTERS_E
{
	A_REG = 0x01, // byte wide accumlator
	X_REG = 0x02, // byte wide, general purpose
	Y_REG = 0x03, // byte wide, 
	PC_REG = 0x04, // program counter
	SP_REG = 0x05, // stack pointer
	P_REG = 0x06 // register used by the ALU, used in logic, arithmetic, and branch instructions.
				// known as the "status register" according to nesdev
};

enum ADDRESSING_MODES_E
{
	ZPI_X = 0x01, // zero page indexed d, x
	ZPI_Y = 0x02, // zero page indexed d, y
	ABSINDEX_X = 0x03, // absolute index d, x
	ABSINDEX_Y = 0x04, // absolute index d, y
	INDEXINDIR_X = 0x05, // index indirect d, x
	INDEXINDIR_Y = 0x06 // index indirect d, y
};

static struct nes_stru
{
	nes_stru() = default;
	void fn_initialize_nes();
	void fn_run_cpu_clock();
	void fn_destroy_nes();

protected:
	opcode_stru cur_opcode;
} g_nes;