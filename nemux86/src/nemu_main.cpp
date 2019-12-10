#include "nemu_main.hpp"
#include "core/nes.hpp"

void c_nemu::fn_setup_file()
{
	this->file.open(this->file_name, std::ios::in | std::ios::binary);

	if (!this->file.is_open())
	{
		std::printf("FAILED TO LOAD ROM %s\n", this->file_name.c_str());
	}

	this->file.seekg(0, this->file.end);
	this->length = static_cast<std::int32_t>(this->file.tellg());
	this->file.seekg(0, this->file.beg);

	this->memory = std::make_unique<std::uint8_t[]>(this->length);

	this->file.read(reinterpret_cast<char*>(this->memory.get()), this->length);
}

void c_nemu::fn_initialize_nemu(std::string& rom_file)
{
	this->file_name = rom_file;
	this->fn_setup_file();
	this->running_on = true;
	g_nes.fn_initialize_nes();
	g_nes.fn_run_cpu_clock();
}

void c_nemu::fn_run_nemu()
{

}

void c_nemu::fn_destroy_nemu() const
{
	g_nes.fn_destroy_nes();
}
