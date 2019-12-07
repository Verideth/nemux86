#pragma once

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

enum FLAGS
{
	C = 0x01, // carry fag
	Z = 0x02, // zero flag
	I = 0x03, // interrupt flag
	D = 0x04, // decimal flag, no effect on NES accord to nesdev
	V = 0x05, // overflow flag, IF ADC OR SBC RETURN AN INVALID RESULT THIS FLAG IS SET
	B = 0x06 // b flag, EXPLICITLY NOT a register, IT IS a flag
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