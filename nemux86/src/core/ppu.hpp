#pragma once

#include <vector>
#include <cstdint>
#include <map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>

#define LEFT_TABLE_START 0x0000 // starts at 0x0000
#define LEFT_TABLE_END 0x0FFF // left table ends at 0x0FFF
#define RIGHT_TABLE_START 0x1000 // right table starts at 0x1000
#define RIGHT_TABLE_END 0x1FFF // right table ends at 0x1FFF

#define SCANLINE_END 321

enum class PIPELINE_STATES_E
{
	PRERENDER,
	RENDER,
	QUIT_RENDER,
	VERTICAL_BLANK
};

/* implement later...? */
enum OAM_READ_FROM_E
{
	PRIMARY_OAM = 1,
	SECONDARY_OAM = 2
};

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
	struct flags_ctrl_stru
	{
		flags_ctrl_stru() = default;
		std::uint8_t bytecode = 0;
		/* HI BITS */
		std::uint8_t v = 1; // generates an NMI if 1. 0 is off.
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
	struct flags_mask_stru
	{
		flags_mask_stru() = default;
		std::uint8_t bytecode = 0;
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
	 * PPUSTATUS
	 * this register, according to nesdev
	 * reflects varius "states" for the
	 * functions inside of the PPU.
	 * apparently its often used for timing.
	 */
	struct flags_status_stru
	{
		flags_status_stru() = default;
		std::uint8_t bytecode = 0;

		/* HI BITS */
		std::uint8_t v = 0; // 0: not in vblank, 1: in vblank,
							// according to nesdev vblank stands
							// for vertical blank, which is a flag
							// for generating a video display to be used
							// by the nes itself.
		std::uint8_t s = 0; // sprite 0 hit. set when a nonzero pixel
							// of sprite index 0 overlaps a nonzero
							// background pixel. used for rastar timing.
							// as well as to see if a sprite could
							// draw on the vertical blank.
							// could be created on to the vertical blank
		std::uint8_t o = 0; // sprite overflow. 0: no 1: yes
							// this flag checks if there are more
							// than eight sprites on a scanline.
							// apparently hardware bugs will make
							// it generate false positives, as
							// well as false negatives.
							// this flag is also set at sprite evals
	} ppu_status;

	/*
	 * OAMADDR
	 * (IMPLEMENT LATER(?))
	 * handles the OAM address port
	 */
	struct flags_read_stru
	{
		flags_read_stru() = default;
		std::uint8_t bytecode = 0;
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
	struct flags_data_stru
	{
		flags_data_stru() = default;
		std::uint8_t bytecode = 0;
	} ppu_oamdata;

	/*
	 * PPUSCROLL
	 * this register handles scrolling of the screen
	 * (aka where you move and the landscape changes as
	 * you move)
	 */
	struct flags_scroll_stru
	{
		flags_scroll_stru() = default;
		std::uint8_t bytecode = 0;
		
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
	struct flags_address_stru
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
		std::uint8_t bytecode = 0;
		
		/* HI BITS */
		std::uint8_t high_byte_write = 0; // high byte to write, in the example above this is 0x21
		/* LO BITS */
		std::uint8_t low_byte_write = 0; // low byte to write, in the example above this is 0x08
	} ppu_addr;

	/*
	 * PPUDATA
	 * VRAM read/write register, after accessing
	 * this register the video memory (VRAM)
	 * will increment by one.
	 */
	struct flags_vramdata_stru
	{
		flags_vramdata_stru() = default;
		std::uint8_t bytecode = 0;
		
		/* HI BITS */
		std::uint8_t high_byte_write = 0;
		/* LO BITS */
		std::uint8_t low_byte_write = 0;
	} ppu_data;

	/*
	 * OAMDMA
	 * the OAMDMA register, according to nesdev, the
	 * port is located on the CPU itself. writing 0xNN
	 * will send 256 bytes from the page 0xNN00 - 0xNNFF
	 * to the PPU OAM address port
	 */
	struct flags_oamdma_stru
	{
		flags_oamdma_stru() = default;
		std::uint8_t bytecode = 0;

		/* HI BITS */
		std::uint16_t high_byte_write = 0;
		/* LO BITS */
		std::uint16_t low_byte_write = 0;

	} ppu_oamdma;
};

/*
 * PATTERN TABLE STRUCTURE
 * this structure is the base structure for how
 * pixels are stored and read inside of the NES.
 *
 * each pattern table holds a set of data
 * for the pixel data. which then draws sprites.
 */
struct pattern_table_stru
{
	pattern_table_stru() = default;
	std::uint8_t what_half = 0; // 0: left
								// 1: right
	std::uint8_t tile_row = 0; // what row the pixel is on
	std::uint8_t column = 0; // what column the pixel lies on
	std::uint8_t bit_plane = 0; // 0: lower
								// 1: upper
	std::uint8_t y_offset = 0;
};

/* ppu structure itself */
static struct nes_ppu_stru
{
	void fn_init_ppu();
	void fn_run_ppu();
	void fn_draw_sprite(sf::Vector2f& position, void* sprite_data);
	void fn_store_sprite(sf::Sprite& sprite_to_store);
	void fn_destroy_ppu();
	void fn_init_registers();
	void fn_do_scroll();
	
	registers_stru registers;
	PIPELINE_STATES_E current_pipeline_state = PIPELINE_STATES_E::PRERENDER;

	// cpu cycle
	std::uint16_t cycle = 0; // the cycle of the PPU
	std::uint16_t scanline = 0; // scans each cycle
	std::vector<std::uint8_t> pattern_table_left;
	std::vector<std::uint8_t> pattern_table_right;

	std::vector<std::vector<std::uint8_t>> pattern_table;

private:
	std::map<std::int32_t, void*> sprite_map_data;
	void fn_get_pattern_data();
	void fn_handle_prerender(); // called before rendering
	void fn_handle_render(); // render
	void fn_handle_quitrender(); // called when render stops
	void fn_handle_vblank(); // vertical blank function
	void fn_breakup_pattern_table();
} g_nes_ppu;