#pragma once

#include <fstream>
#include <cstdint>
#include "core/nes.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class c_nemu
{
public:
	c_nemu() : length(0) {  }
	void fn_initialize_nemu(std::string& rom_file);
	void fn_run_nemu();
	void fn_destroy_nemu() const;
	void fn_setup_file();
	bool running_on = false;
	std::ifstream file;
	std::int32_t length;
	std::unique_ptr<std::uint8_t[]> memory;
	std::string file_name;

private:
	static std::unique_ptr<sf::RenderWindow> window;
};

extern std::unique_ptr<c_nemu> nemu_ptr;