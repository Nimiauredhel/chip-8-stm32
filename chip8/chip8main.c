#include "chip8main.h"

int chip8start(void)
{
	int16_t rom_index = -1;

    bool should_run = true;

    while (should_run)
    {
    	rom_index++;
    	if (rom_index > CHIP_8_PROGRAM_COUNT) rom_index = 0;

		Chip8_t *chip8 = create_instance(chip8_programs[rom_index]);
		bool should_run = run(chip8);
		free(chip8);
    }

    return EXIT_SUCCESS;
}
