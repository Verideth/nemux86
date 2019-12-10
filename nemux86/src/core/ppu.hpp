#pragma once

#include <vector>
#include <cstdint>
#include <map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>

enum PIPELINE_STATES
{
	
};

namespace PPU_FUNCTIONS
{
	inline void gfn_read_ppu_x(std::int16_t address)
	{
		
	}
}

static struct nes_ppu_stru
{
	void fn_init_ppu();
	void fn_run_ppu();
	void fn_do_scroll();
	void fn_draw_sprite(sf::Vector2f position, void* sprite_data);
	void fn_store_sprite(std::vector<sf::Sprite> vector_to_store);
	void fn_get_pattern_data();

	std::int16_t ppu_ctrl_reg = 0;
	std::int16_t ppu_mask_reg = 0;
	std::int16_t ppu_status_reg = 0;
	std::int16_t ppu_oadmaddr_reg = 0;
	std::int16_t ppu_scroll_reg = 0;
	std::int16_t ppu_addr_reg = 0;
	std::int16_t ppu_data_reg = 0;

	std::uint16_t cur_cycle = 0;
	
	sf::Vector2f scroll_position { 0.0, 0.0 };

	std::vector<sf::Sprite> sprites;
	std::vector<sf::Sprite> sprites_to_draw;
	std::map<std::int32_t, void*> sprite_map_data;
} g_nes_ppu;