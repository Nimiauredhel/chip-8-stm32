#include "chip8main.h"

int chip8start(void)
{
    char *rom_path;
    bool should_run = true;
    while (should_run)
    {
		Chip8_t *chip8 = create_instance(test_program);
		bool should_run = run(chip8);
		free(chip8);
    }

    return EXIT_SUCCESS;
}
