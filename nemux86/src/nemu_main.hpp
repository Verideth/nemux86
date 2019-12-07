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
	void initialize_nemu(std::string& rom_file);
	void run_nemu();
	void destroy_nemu();
	void setup_file();
	bool running_on = false;
	std::ifstream file;
	std::int32_t length;
	std::unique_ptr<std::uint8_t[]> memory;
	std::string file_name;
	c_nes nes_obj;

private:
	static std::unique_ptr<sf::RenderWindow> window;
};

extern std::unique_ptr<c_nemu> nemu_ptr;