#include "ppu.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "../nemu_main.hpp"

sf::Texture texture;
sf::RectangleShape rect;
sf::Texture texture_2;
sf::RectangleShape rect_2;
sf::Sprite sprite;

void nes_ppu_stru::fn_init_ppu()
{
	
}

void nes_ppu_stru::fn_handle_prerender()
{
	this->current_pipeline_state = PIPELINE_STATES_E::RENDER;
	this->fn_init_registers();
	this->fn_get_pattern_data();
}

void nes_ppu_stru::fn_handle_quitrender()
{

}

void nes_ppu_stru::fn_handle_vblank()
{
	// handle vblank stuff here
}

void nes_ppu_stru::fn_run_ppu()
{
	switch (this->current_pipeline_state)
	{
	case PIPELINE_STATES_E::PRERENDER:
		this->fn_handle_prerender();
		break;
	case PIPELINE_STATES_E::RENDER:
		this->fn_handle_render();
		break;
	case PIPELINE_STATES_E::QUIT_RENDER:
		this->fn_destroy_ppu();
		break;
	case PIPELINE_STATES_E::VERTICAL_BLANK:
		this->fn_handle_vblank();
		break;
	default:
		break;
	}
}

void nes_ppu_stru::fn_do_scroll()
{
	
}

void nes_ppu_stru::fn_draw_sprite(sf::Vector2f& position, void* sprite_data)
{
	
}

void nes_ppu_stru::fn_store_sprite(sf::Sprite& sprite_to_store)
{
	
}

void nes_ppu_stru::fn_get_pattern_data()
{
	for (std::uint16_t it = LEFT_TABLE_START;
		it <= LEFT_TABLE_END;
		it++)
	{
		auto scanline = g_nemu_ptr->memory[it];

		if (scanline)
		{
			this->pattern_table_left.push_back(scanline);
		}
	}

	for (std::uint16_t it = RIGHT_TABLE_START;
		it <= RIGHT_TABLE_END - 1;
		it++)
	{
		auto scanline = g_nemu_ptr->memory[it];

		if (scanline)
		{
			this->pattern_table_right.push_back(scanline);
		}
	}

	this->fn_breakup_pattern_table();
}

void nes_ppu_stru::fn_breakup_pattern_table()
{
	/*
	* Bit plane top
	* $0xx0=$41  01000001
	* $0xx1=$C2  11000010
	* $0xx2=$44  01000100
	* $0xx3=$48  01001000
	* $0xx4=$10  00010000
	* $0xx5=$20  00100000         .1.....3
	* $0xx6=$40  01000000         11....3.
	* $0xx7=$80  10000000  =====  .1...3..
    * Bit plane bottom			  .1..3...
	* $0xx8=$01  00000001  =====  ...3.22.
	* $0xx9=$02  00000010         ..3....2
	* $0xxA=$04  00000100         .3....2.
	* $0xxB=$08  00001000         3....222
	* $0xxC=$16  00010110
	* $0xxD=$21  00100001
	* $0xxE=$42  01000010
	* /
	// if bit at X in plane 1 is on, and bit at X in plane Y is on, then 3
	// if bit at X in plane 1 is on, and bit at X in plane Y is off, then 1
	// if bit set has any other bits in it, then bit is 2, assuming other bit doesnt have
	// one at same location

	/*
	* 01000001 // A
	* 00000001 // B
	* --------
	* 01000003 // C		<-- array bytes
	* --------
	*/
	
	std::vector<std::vector<std::uint8_t>> bytecode_data;
	const std::uint8_t number_of_bytes_pattern = 16; // probably varies?

	// for each 16 bytes in the pattern table...
	for (std::uint16_t l_palette_mem_ptr = LEFT_TABLE_START; l_palette_mem_ptr < LEFT_TABLE_END - 1; l_palette_mem_ptr += number_of_bytes_pattern)
	{
		/* extract top and bottom bit planes. */
		std::uint8_t top_bit_plane[8]; // A
		std::uint8_t bot_bit_plane[8]; // B

		// for these 16 bytes in the tile.
		for (uint8_t byte_index = 0; 
			byte_index < number_of_bytes_pattern; 
			byte_index++)
		{	
			if (byte_index < 8) // 0, 1, 2, 3, 4, 5, 6, 7
			{
				// first bit plane.
				top_bit_plane[byte_index] = this->pattern_table_left[byte_index]; 
			}
			else if (byte_index >= 8) 	// 8, 9, 10, 11, 12, 13, 14, 15
			{
				// second bit plane.
				const std::uint8_t offset_byte_index = byte_index - 8; // this is so we start at index 0.

				bot_bit_plane[offset_byte_index] = this->pattern_table_left[byte_index];
			}
		}

		l_palette_mem_ptr++;
		

		
		/* combine the top and bottom bit plane to get the pixel pattern */
		
		// the resulting pixel pattern is
		std::vector<std::uint8_t> pixel_pattern; // C = A + B

		for (std::uint8_t byte_index = 0; 
			byte_index < 8; 
			byte_index++)
		{
			const std::int8_t top_plane_byte = top_bit_plane[byte_index]; // 0 or 1, bit index 0
			const std::int8_t bot_plane_byte = bot_bit_plane[byte_index]; // 0 or 1, bit index 1

			// we start at the far left hand side of the byte
			for (std::uint32_t target_bit = 0; 
				target_bit < 8; 
				target_bit++) // 0, 1, 2, 3, 4, 5, 6, 7
			{
				// we start at the far left hand side of the byte and scan right.
				const std::uint8_t top_plane_bit = (top_plane_byte >> (7 - target_bit)) & (0b00000001); // pixel plane top controls bit 0
				const std::uint8_t bot_plane_bit = (bot_plane_byte >> (7 - target_bit)) & (0b00000010); // pixel plane bottom controls bit 1

				// combine them
				uint8_t pixel_pattern_byte = 0b00000000;
				pixel_pattern_byte = pixel_pattern_byte | top_plane_bit;
				pixel_pattern_byte = pixel_pattern_byte | bot_plane_bit;

				// we start at the far left hand side of the byte and scanning right, add to this table left to right.
				pixel_pattern.push_back(pixel_pattern_byte);
			}

			bytecode_data.push_back(pixel_pattern);
			this->pattern_table.push_back(bytecode_data[byte_index]);
		}
	}
	
	std::printf("PATTERN TABLE SIZE = %i\n", this->pattern_table.size());
}

std::int32_t it = 0;
void nes_ppu_stru::fn_handle_render()
{
	// this isn't where the error is, its in the function above ^
	// yea i know, im just seeing what happens with different
	// variables to better understand
	//
	
	++it;
	if (it == 255) { it = 0; }

	const std::uint8_t* test_data = this->pattern_table[6].data();
	texture.create(80.0, 80.0);
	texture.update(test_data);
	texture.generateMipmap();
	texture.setSrgb(true);
	
	sprite.setTexture(texture);
	sprite.setPosition(sf::Vector2f(0.0, 0.0));
	sprite.setScale(3.0, 3.0);
	
	g_nemu_ptr->window.draw(sprite);

	this->fn_do_scroll();
}


void nes_ppu_stru::fn_destroy_ppu()
{
	this->cycle = 0;
	this->current_pipeline_state = PIPELINE_STATES_E::QUIT_RENDER;
	this->scanline = 0;
	g_nemu_ptr->window.close();
}

void nes_ppu_stru::fn_init_registers()
{
	constexpr std::uint8_t shift_amt = 7; // we isolate it to the end, subtracted by the offset, so the value is that.

	/*
	 * EACH register inside of the PPU starts at
	 * 0x2000 (PPUCTRL), and ends at 0x2007 (PPUDATA). hence what I
	 * am doing here.
	 * OAMDMA is a special register, initiated at 0x4014
	 * according to nesdev
	 */
	
	/* PPUCTRL IS INITIALIZED HERE */
	this->registers.ppu_ctrl.bytecode = (g_nemu_ptr->memory[0x2000]);
	/* hi bits */
	this->registers.ppu_ctrl.v = (this->registers.ppu_ctrl.bytecode & 0b10000000) >> (shift_amt - 0);
	this->registers.ppu_ctrl.master_save_select = (this->registers.ppu_ctrl.bytecode & 0b01000000) >> (shift_amt - 1);
	this->registers.ppu_ctrl.sprite_size = (this->registers.ppu_ctrl.bytecode & 0b00100000) >> (shift_amt - 2);
	this->registers.ppu_ctrl.background_pattern_table = (this->registers.ppu_ctrl.bytecode & 0b00010000) >> (shift_amt - 3);
	/* lo bits */
	this->registers.ppu_ctrl.sprite_pattern_table = (this->registers.ppu_ctrl.bytecode & 0b00001000) >> (shift_amt - 4);
	this->registers.ppu_ctrl.vram_incrementer = (this->registers.ppu_ctrl.bytecode & 0b00000100) >> (shift_amt - 5);
	/* lowest 2 bits, nametable address */
	this->registers.ppu_ctrl.nametable_addresses = (this->registers.ppu_ctrl.bytecode & 0b000000011); // no shift needed because its the end bits

	/* PPUMASK IS INITIALIZED HERE */
	this->registers.ppu_mask.bytecode = (g_nemu_ptr->memory[0x2001]);
	/* hi bits */
	this->registers.ppu_mask.blue = (this->registers.ppu_mask.bytecode & 0b10000000) >> (shift_amt - 0);
	this->registers.ppu_mask.green = (this->registers.ppu_mask.bytecode & 0b01000000) >> (shift_amt - 1);
	this->registers.ppu_mask.red = (this->registers.ppu_mask.bytecode & 0b00100000) >> (shift_amt - 2);
	this->registers.ppu_mask.show_sprites = (this->registers.ppu_mask.bytecode & 0b00010000) >> (shift_amt - 3);
	/* lo bits */
	this->registers.ppu_mask.show_background = (this->registers.ppu_mask.bytecode & 0b00001000) >> (shift_amt - 4);
	this->registers.ppu_mask.show_sprites_leftmost_screen = (this->registers.ppu_mask.bytecode & 0b00000100) >> (shift_amt - 5);
	this->registers.ppu_mask.show_background_leftmost_screen = (this->registers.ppu_mask.bytecode & 0b00000010) >> (shift_amt - 6);
	this->registers.ppu_mask.grayscale = (this->registers.ppu_mask.bytecode & 0b00000001) >> (shift_amt - 7);

	/* PPUSTATUS IS INITALIZED HERE */
	this->registers.ppu_status.bytecode = (g_nemu_ptr->memory[0x2002]);
	/* hi bits */
	this->registers.ppu_status.v = (this->registers.ppu_status.bytecode & 0b10000000) >> (shift_amt - 0);
	this->registers.ppu_status.s = (this->registers.ppu_status.bytecode & 0b01000000) >> (shift_amt - 1);
	this->registers.ppu_status.o = (this->registers.ppu_status.bytecode & 0b00100000) >> (shift_amt - 2);

	/* OAMADDR INTIALIZED HERE */
	this->registers.ppu_oamaddr.bytecode = (g_nemu_ptr->memory[0x2003]);
	/* address to write to */
	this->registers.ppu_oamaddr.write_address = (this->registers.ppu_oamaddr.bytecode & 0b11111111);

	/* OAMDATA INTIALIZED HERE */
	this->registers.ppu_oamdata.bytecode = (g_nemu_ptr->memory[0x2004]);

	/* PPUSCROLL INTIALIZED HERE */
	this->registers.ppu_scroll.bytecode = (g_nemu_ptr->memory[0x2005]);
	/* hi bits */
	this->registers.ppu_scroll.x = (this->registers.ppu_scroll.bytecode & 0b11110000) >> (shift_amt - 4);
	/* lo bits */
	this->registers.ppu_scroll.y = (this->registers.ppu_scroll.bytecode & 0b00001111);

	/* PPUADDR INITALIZED HERE */
	this->registers.ppu_addr.bytecode = (g_nemu_ptr->memory[0x2006]);
	/* hi bits */
	this->registers.ppu_addr.high_byte_write = (this->registers.ppu_addr.bytecode & 0b11110000) >> (shift_amt - 4);
	/* lo bits */
	this->registers.ppu_addr.low_byte_write = (this->registers.ppu_addr.bytecode & 0b00001111);

	/* PPUDATA INITIALIZED HERE */
	this->registers.ppu_data.bytecode = (g_nemu_ptr->memory[0x2007]);
	/* hi bits */
	this->registers.ppu_data.high_byte_write = (this->registers.ppu_data.bytecode & 0b11110000) >> (shift_amt - 4);
	/* lo bits */
	this->registers.ppu_data.low_byte_write = (this->registers.ppu_data.bytecode & 0b00001111);

	/* OAMDMA INTIALIZED HERE */
	this->registers.ppu_oamdma.bytecode = (g_nemu_ptr->memory[0x4014]); // location of OAMDMA
	/* hi bits */
	this->registers.ppu_oamdma.high_byte_write = (this->registers.ppu_oamdma.bytecode & 0b11110000) >> (shift_amt - 4);
	/* lo bits*/
	this->registers.ppu_oamdma.low_byte_write = (this->registers.ppu_oamdma.bytecode & 0b00001111);
}