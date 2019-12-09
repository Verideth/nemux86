#include <memory>
#include "nemu_main.hpp"
#include <cstdint>

std::unique_ptr<c_nemu> nemu_ptr = std::make_unique<c_nemu>();

std::int32_t main(std::int32_t argc, char* argv[])
{
	std::string name = "Super Mario Bros. (Japan, USA).nes";
	nemu_ptr->fn_initialize_nemu(name);
	nemu_ptr->running_on = true;

	while (nemu_ptr->running_on == true)
	{
		nemu_ptr->fn_run_nemu();
	}

	nemu_ptr->fn_destroy_nemu();

	return 1;
}