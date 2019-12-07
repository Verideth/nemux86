#include "nes.hpp"
#include <cstdint>
#include "../nemu_main.hpp"

void c_nes::initialize_nes()
{

}

void c_nes::run_cpu_clock()
{
	for (std::uint16_t loc_iterator = 0; loc_iterator < nemu_ptr->length; loc_iterator++)
	{
		nes_cpu.convert_mem_bytecode(loc_iterator);
	}

	nes_cpu.setup_opcode_vector();
}

void c_nes::destroy_nes()
{
	// called on stop
}
