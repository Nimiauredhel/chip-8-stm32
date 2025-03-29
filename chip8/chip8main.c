#include "common.h"
#include "chip8_defs.h"
#include "data.h"
#include "emulator.h"

int chip8start()
{
    initialize_random_seed();


    char *rom_path;
    bool should_run = true;
    while (should_run)
    {
		Chip8_t *chip8 = create_instance(rom_path);
		bool should_run = run(chip8);
		free(chip8);
    }

    return EXIT_SUCCESS;
}
