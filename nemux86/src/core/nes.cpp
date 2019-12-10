#include "nes.hpp"
#include <cstdint>
#include "../nemu_main.hpp"
#include "nes_cpu.hpp"

void nes_stru::fn_initialize_nes()
{

}

void nes_stru::fn_run_cpu_clock()
{
	for (std::uint16_t loc_iterator = 0; loc_iterator < g_nemu_ptr->length; loc_iterator++)
	{
		g_nes_cpu.fn_convert_mem_bytecode(loc_iterator);
	}

	std::printf("%i\n", g_nes_cpu.fn_read_value(0x2000, 0x2007));

	g_nes_cpu.fn_setup_opcode_vector();
}

void nes_stru::fn_destroy_nes()
{
	// called on stop
}
