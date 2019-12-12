#include "nes.hpp"
#include <cstdint>
#include "../nemu_main.hpp"
#include "nes_cpu.hpp"
#include "ppu.hpp"

void nes_stru::fn_initialize_nes()
{
	g_nes_cpu.fn_initialize_cpu();
	g_nes_ppu.fn_init_ppu();
}

void nes_stru::fn_run_cpu_clock()
{

}

void nes_stru::fn_destroy_nes()
{

}
