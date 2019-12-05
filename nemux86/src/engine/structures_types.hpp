#pragma once

#include <cstdint>

struct vertex_t
{
	std::int16_t x;
	std::int16_t y;
};

struct sprite_t
{
	void* sprite_data;
	std::int16_t x_pos;
	std::int16_t y_pos;
};