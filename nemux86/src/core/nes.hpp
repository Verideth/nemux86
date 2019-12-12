#pragma once

#include "opcodes.hpp"

static struct nes_stru
{
	nes_stru() = default;
	void fn_initialize_nes();
	void fn_run_cpu_clock();
	void fn_destroy_nes();

protected:
	opcode_stru cur_opcode;
} g_nes;