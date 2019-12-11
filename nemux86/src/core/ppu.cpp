#include "ppu.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "../nemu_main.hpp"

void nes_ppu_stru::fn_init_ppu()
{
	this->fn_init_registers();
}

void nes_ppu_stru::fn_run_ppu()
{
	
}

void nes_ppu_stru::fn_do_scroll()
{
	
}

void nes_ppu_stru::fn_draw_sprite(sf::Vector2f& position, void* sprite_data)
{
	
}

void nes_ppu_stru::fn_store_sprite(std::vector<sf::Sprite>& vector_to_store)
{
	
}

void nes_ppu_stru::fn_get_pattern_data()
{
	
}

void nes_ppu_stru::fn_init_registers()
{
	const std::uint8_t shift_amt = 7; // we isolate it to the end, subtracted by the offset, so the value is that.

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