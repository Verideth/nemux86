#include "ppu.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "../nemu_main.hpp"
#include <array>
#include <iterator>

void nes_ppu_stru::fn_init_ppu()
{
	this->fn_init_registers();
	this->fn_get_pattern_data();
}

void nes_ppu_stru::fn_handle_prerender()
{
	this->current_pipeline_state = PIPELINE_STATES_E::RENDER;
}

void nes_ppu_stru::fn_handle_render()
{
	// handle rendering
	
	this->fn_do_scroll();
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
	std::int32_t byte_check = 0; // total bytes checked from both pattern tables
	std::int32_t left_checks = 0; // total bytes checked from the left table
	std::int32_t right_checks = 16; // total bytes checked from right table
	std::vector<std::uint8_t> bytecode_data;

	for (std::uint16_t it = LEFT_TABLE_START;
		it <= LEFT_TABLE_END - 1;
		++it)
	{
		/* if 16 bytes has passed on the left pattern table */
		if (left_checks == 16)
		{
			bytecode_data.push_back(this->pattern_table_left[it]);
			++left_checks;
		}
		/* increment right side when left is over */
		else if (left_checks >= 16)
		{
			bytecode_data.push_back(this->pattern_table_right[it]);
			++right_checks;

			/* if 16 bytes has passed on the right table */
			if (right_checks == 32)
			{
				right_checks = 16;
				left_checks = 0;
			}
		}

		/*
		 * once the checking is done processing,
		 * send all the data over into the
		 * main pattern table vector!
		 */
		if (byte_check == 32)
		{
			this->pattern_table.push_back(bytecode_data);
			byte_check = 0;
		}
		
		++byte_check;
	}

	bytecode_data.clear();
	std::printf("PATTERN TABLE SIZE = %i\n", this->pattern_table.size());
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