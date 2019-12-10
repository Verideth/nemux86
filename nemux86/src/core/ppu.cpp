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
	PPUCTRL IS INITIALIZED HERE
	*/
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
	this->registers.ppu_ctrl.nametable_addresses = (this->registers.ppu_ctrl.bytecode & 0b000000011);
}