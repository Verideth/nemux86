#include <memory>
#include "nemu_main.hpp"
#include <cstdint>

std::unique_ptr<c_nemu> g_nemu_ptr = std::make_unique<c_nemu>();

std::int32_t main(std::int32_t argc, char* argv[])
{
	std::string name = "Super Mario Bros. (Japan, USA).nes";
	g_nemu_ptr->fn_initialize_nemu(name);
	g_nemu_ptr->running_on = true;

	while (g_nemu_ptr->running_on == true)
	{
		g_nemu_ptr->fn_run_nemu();
	}

	g_nemu_ptr->fn_destroy_nemu();

	return 1;
}