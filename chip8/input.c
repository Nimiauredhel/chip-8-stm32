#include "input.h"

char chip8_key_names[CHIP8_KEY_COUNT][4] =
{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "Any" };

char emu_key_names[EMU_KEY_COUNT][10] =
{ "Quit", "Reset", "Speed +", "Speed -", "Step Mode", "Step One", "Any" };

void read_input(uint16_t *emu_key_states, uint16_t *chip8_key_states)
{
#define REGISTER_EMU_KEY_DOWN(key)\
	emu_key_states[key] = EMU_INPUT_BUFFER_CYCLES;\
	emu_key_states[EMU_KEY_ANY_IDX] = EMU_INPUT_BUFFER_CYCLES
#define REGISTER_CHIP8_KEY_DOWN(key)\
	chip8_key_states[key] = CHIP8_INPUT_BUFFER_CYCLES;\
	chip8_key_states[CHIP8_KEY_ANY_IDX] = CHIP8_INPUT_BUFFER_CYCLES

    uint8_t idx;
    uint16_t key_state_current = keypad_get_state();

    for (idx = 0; idx < CHIP8_KEY_COUNT; idx++)
    {
    	if (key_state_current & 1 << idx)
    	{
    		REGISTER_CHIP8_KEY_DOWN(idx);
    	}
    	else if (chip8_key_states[idx] > 0)
        chip8_key_states[idx] -= 1;
    }

    /*
    for (idx = 0; idx < EMU_KEY_COUNT; idx++)
    {
        if (emu_key_states[idx] > 0)
        emu_key_states[idx] -= 1;
    }
    */

#undef REGISTER_EMU_KEY_DOWN
#undef REGISTER_CHIP8_KEY_DOWN
}

bool check_input(uint16_t *key_states, uint8_t key_idx)
{
    bool result = key_states[key_idx] > 0;
    return result;
}

uint16_t check_input_buffer(uint16_t *key_states, uint8_t key_idx)
{
    uint8_t result = key_states[key_idx];
    return result;
}
