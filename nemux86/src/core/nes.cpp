#include "nes.hpp"
#include <cstdint>
#include "../nemu_main.hpp"
#include "nes_cpu.hpp"
#include "ppu.hpp"

void nes_stru::fn_initialize_nes()
{
	g_nes_ppu.fn_init_ppu();
}

void nes_stru::fn_run_cpu_clock()
{
	for (std::uint16_t loc_iterator = 0; loc_iterator < g_nemu_ptr->length; loc_iterator++)
	{
		g_nes_cpu.fn_convert_mem_bytecode(loc_iterator);
	}
	
	g_nes_cpu.fn_setup_opcode_vector();
}

void nes_stru::fn_destroy_nes()
{
	// called on stop
}
