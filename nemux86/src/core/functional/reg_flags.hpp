#pragma once

enum REGISTERS
{
	A_REG, // byte wide accumlator
	X_REG, // byte wide, general purpose
	Y_REG, // byte wide, 
	PC_REG, // program counter
	SP_REG, // stack pointer
	P_REG // register used by the ALU, used in logic, arithmetic, and branch instructions.
		// known as the "status register" according to nesdev
};