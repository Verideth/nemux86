#include "nes.hpp"
#include <cstdint>
#include "../nemu_main.hpp"

void c_nes::initialize_nes()
{
	c_nes_cpu* global_nes_cpu_ptr = new c_nes_cpu;
}

void c_nes::run_cpu_clock()
{
	for (std::int16_t it_addr = 0; it_addr < nemu_ptr->length; it_addr++)
	{
		global_nes_cpu_ptr->convert_mem_bytecode(it_addr);
	}
}

void c_nes::destroy_nes()
{

}