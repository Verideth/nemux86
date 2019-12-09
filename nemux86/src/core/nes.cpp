#include "nes.hpp"
#include <cstdint>
#include "../nemu_main.hpp"

void nes_str::fn_initialize_nes()
{

}

void nes_str::fn_run_cpu_clock()
{
	for (std::uint16_t loc_iterator = 0; loc_iterator < nemu_ptr->length; loc_iterator++)
	{
		nes_cpu.fn_convert_mem_bytecode(loc_iterator);
	}

	nes_cpu.fn_setup_opcode_vector();
}

void nes_str::fn_destroy_nes()
{
	// called on stop
}
