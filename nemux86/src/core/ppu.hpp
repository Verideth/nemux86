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

/* ppu register structure, for handling register objects */
struct registers_stru
{
	/*
	 * PPUCTRL
	 * this register handles basic ppu controlling flags
	 * according to nes dev its just general data and
	 * information about various controlling flags
	 * inside of the ppu
	 */
	static struct flags_ctrl_stru
	{
		flags_ctrl_stru() = default;
		/* HI BITS */
		std::uint8_t v = 0; // generates an NMI if 1. 0 is off.
		std::uint8_t master_save_select = 0; // 0 read color from EXT pins, 1 output color on EXT pins
		std::uint8_t sprite_size = 0; // 8x8 pixels, or 8x16 pixels
		std::uint16_t background_pattern_table = 0; // 0: 0x1000, 1: 0x2000
		
		/* LO BITS */
		std::uint16_t sprite_pattern_table = 0; // 0: 0x0000, 1: 0x1000, ignored in 8x16 mode on the sprite
		std::uint16_t vram_incrementer = 0; // vram address incrementer per CPU read/write

		/* LOWEST 2 BITS, 0xXYNN */
		std::uint16_t nametable_addresses = 0; // 0: 0x2000, 1: 0x2400
											   // 2(?): 0x2800, 3(?): 0x2C00
	} ppu_ctrl;

	/*
	 * PPUMASK
	 * this register handles sprite coloring, backgrounds
	 * and according to nesdev, color effects as well.
	 */
	static struct flags_mask_stru
	{
		flags_mask_stru() = default;
		/* HI BITS */
		std::uint8_t blue = 0; // blue color
		std::uint8_t green = 0; // green color
		std::uint8_t red = 0; // red color
		std::uint8_t show_sprites = 0; // show sprites on screen

		/* LO BITS */
		std::uint8_t show_background = 0; // show background
		std::uint8_t show_sprites_leftmost_screen = 0; // show sprites at the leftmost (8 pixels)
													   // screen 0: hide, 1: show
		std::uint8_t show_background_leftmost_screen = 0; // show background at leftmost (8 pixels)
														  // of screen, 0: hide, 1: show
		std::uint8_t grayscale = 0; // grayscale the colors. 0: off, 1: on
	} ppu_mask;

	/*
	 * OAMADDR
	 * (IMPLEMENT LATER(?))
	 * handles the OAM address port
	 */
	static struct flags_read_stru
	{
		flags_read_stru() = default;
		std::int16_t write_address = 0x0000; // the address you want the OAM to write to
		
		/* NESDEV didn't give much info but apparently
		 * it writes to the address you want to access here
		 * i'm not fully sure about this, but it said most games
		 * write to OAMDMA (which is another register in the 0x4014 range
		 */
	} ppu_oamaddr;

	/*
	 * OAMDATA
	 * handling of writing and reading of the OAM
	 * are done in this register, starting from the set
	 * OAMADDR address
	 * wries to this will increment the OAMADDR
	 */
	static struct flags_data_stru
	{
		flags_data_stru() = default;
		
	} ppu_oamdata;

	/*
	 * PPUSCROLL
	 * this register handles scrolling of the screen
	 * (aka where you move and the landscape changes as
	 * you move)
	 */
	static struct flags_scroll_stru
	{
		flags_scroll_stru() = default;
		std::int16_t x = 0; // x value of the scroll position
		std::int16_t y = 0; // y value of the scroll position
	} ppu_scroll;

	/*
	 * PPUADDR
	 * in the NES, the CPU and PPU are on seperate buses
	 * so this register allows the CPU to write to VRAM
	 * using this register.
	 *
	 * first, it loads the address into PPUADDR, then
	 * uses this to write to it.
	 */
	static struct flags_address_stru
	{
		/*
		 * EXAMPLE OF IT IN USE ON 6502 ASSEMBLER:
		 *
		 * lda #$21
		 * sta PPUADDR
		 * lda #$08
		 * sta PPUADDR
		 *
		 * this will set the vram address to 0x2108
		 */

		flags_address_stru() = default;
		
		/* HI BITS */
		std::uint8_t high_byte_write = 0; // high byte to write, in the example above this is 0x21
		/* LO BITS */
		std::uint8_t low_bytes_write = 0; // low byte to write, in the example above this is 0x08
	} ppu_ppuaddr;

	/*
	 * PPUDATA
	 * VRAM read/write register, after accessing
	 * this register the video memory (VRAM)
	 * will increment by one.
	 */
	static struct flags_vramdata_stru
	{
		flags_vramdata_stru() = default;
		
		/* HI BITS */
		std::uint8_t high_byte_write = 0;
		/* LO BITS */
		std::uint8_t low_byte_write = 0;
	} ppu_ppudata;

	/*
	 * OAMDMA
	 * the OAMDMA register, according to nesdev, the
	 * port is located on the CPU itself. writing 0xNN
	 * will send 256 bytes from the page 0xNN00 - 0xNNFF
	 * to the PPU OAM address port
	 */
	static struct flags_oamdma_stru
	{
		flags_oamdma_stru() = default;

		/* HI BITS */
		std::uint16_t high_byte_write = 0;
		/* LO BITS */
		std::uint16_t low_byte_write = 0;

	} ppu_oamdma;
};

/* ppu structure itself */
static struct nes_ppu_stru
{
	void fn_init_ppu();
	void fn_run_ppu();
	void fn_do_scroll();
	void fn_draw_sprite(sf::Vector2f& position, void* sprite_data);
	void fn_store_sprite(std::vector<sf::Sprite>& vector_to_store);
	void fn_get_pattern_data();

	std::int16_t ppu_ctrl_reg = 0;
	std::int16_t ppu_mask_reg = 0;
	std::int16_t ppu_status_reg = 0;
	std::int16_t ppu_oadmaddr_reg = 0;
	std::int16_t ppu_scroll_reg = 0;
	std::int16_t ppu_addr_reg = 0;
	std::int16_t ppu_data_reg = 0;

	std::uint16_t cur_cycle = 0;
	registers_stru registers;

	std::vector<sf::Sprite> sprites;
	std::vector<sf::Sprite> sprites_to_draw;
	std::map<std::int32_t, void*> sprite_map_data;
} g_nes_ppu;
