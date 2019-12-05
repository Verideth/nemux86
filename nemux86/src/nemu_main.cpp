#include "nemu_main.hpp"

c_nemu::c_nemu() 
{ 
	this->length = 0; 
}

void c_nemu::setup_file()
{
	this->file.open(this->file_name, std::ios::in | std::ios::binary);

	if (!this->file.is_open()) { std::printf("FAILED TO LOAD ROM %s\n", this->file_name.c_str()); }

	this->file.seekg(0, this->file.end);
	this->length = this->file.tellg();
	this->file.seekg(0, this->file.beg);

	this->memory = std::make_unique<std::uint8_t[]>(this->length);

	this->file.read(reinterpret_cast<char*>(this->memory.get()), this->length);
}

void c_nemu::initialize_nemu(std::string& rom_file)
{
	this->file_name = rom_file;
	this->setup_file();
	this->running_on = true;
	this->nes_obj.run_cpu_clock();
}

void c_nemu::run_nemu()
{

}