#include "renderer.h"

#define REG8MID 64
#define REG8HIGH 128
#define REG16MID 16384
#define REG16HIGH 32768


#define REG_CONDITIONAL_COLOR_ON(window, reg, mid_thresh, high_thresh) \
    color_pair_idx = reg > mid_thresh ? reg > high_thresh ? COLOR_PAIR_TEXT_RED : COLOR_PAIR_TEXT_YELLOW : 0; \
    if (color_pair_idx > 0) wattron(window, COLOR_PAIR(color_pair_idx))

#define KEY_CONDITIONAL_COLOR_ON(window, key) \
    color_pair_idx = emu_state->chip8_key_states[key] > 0 ? COLOR_PAIR_BG_YELLOW : 0; \
    if (color_pair_idx > 0) wattron(window, COLOR_PAIR(color_pair_idx))

#define COLOR_OFF(window) \
    if (color_pair_idx > 0) wattroff(window, COLOR_PAIR(color_pair_idx)); \
    color_pair_idx = 0

void init_display(DisplayLayout_t *layout)
{
	gfx_init(0);
	layout->window_chip8 = gfx_bytes_to_binary_sprite(32, 8, NULL);
}

void render_display(Chip8_t *chip8, WINDOW *window_chip8)
{
	gfx_bytes_to_binary_sprite_nonalloc(window_chip8, 32, 8, chip8->display_memory);
	gfx_fill_screen(color_blue);
	gfx_draw_binary_sprite(window_chip8, 32, 26, color_green, 4);
	gfx_push_to_screen();
}

void render_disassembly(Chip8Instruction_t *instruction, WINDOW *window_disassembly)
{
	/*
    static char line_memory[16][16] = {0};
    static int8_t tail_idx = 0;
    int8_t idx = tail_idx;

    snprintf_instruction(line_memory[tail_idx], 16, instruction);

    werase(window_disassembly);

    wattron(window_disassembly, COLOR_PAIR(COLOR_PAIR_BG_YELLOW));
    mvwprintw(window_disassembly, 3, 2, "%s", line_memory[idx]);
    wattroff(window_disassembly, COLOR_PAIR(COLOR_PAIR_BG_YELLOW));

    for (uint8_t i = 1; i < 16; i++)
    {
        idx = tail_idx - i;
        if (idx < 0) idx += 16;
        wattron(window_disassembly, COLOR_PAIR(COLOR_PAIR_TEXT_MAGENTA + (idx % 2)));
        mvwprintw(window_disassembly, 3-(i%4), 2 + (12*(i/4)), "%s", line_memory[idx]);
        wattroff(window_disassembly, COLOR_PAIR(COLOR_PAIR_TEXT_MAGENTA + (idx % 2)));
    }

    tail_idx++;
    if (tail_idx > 15) tail_idx = 0;

    wrefresh(window_disassembly);
    */
}

void render_registers(Chip8Registers_t *registers, WINDOW *window_registers)
{
	/*
    int8_t color_pair_idx = 0;

    werase(window_registers);
    box(window_registers, 0, 0);

    for (int i = 0; i < 16; i++)
    {
        REG_CONDITIONAL_COLOR_ON(window_registers, registers->V_REGS[i], REG8MID, REG8HIGH);
        mvwprintw(window_registers, i+1, 1, "V%d[%u]", i, registers->V_REGS[i]);
        COLOR_OFF(window_registers);
        REG_CONDITIONAL_COLOR_ON(window_registers, registers->STACK_RET[i], REG16MID, REG16HIGH);
        mvwprintw(window_registers, i+1, 49, "STACK_RET%d[%u]", i, registers->STACK_RET[i]);
        COLOR_OFF(window_registers);
    }

    REG_CONDITIONAL_COLOR_ON(window_registers, registers->PC, REG16MID, REG16HIGH);
    mvwprintw(window_registers, 1, 9, "PC[%u] ", registers->PC);
    COLOR_OFF(window_registers);
    REG_CONDITIONAL_COLOR_ON(window_registers, registers->SP, REG8MID, REG8HIGH);
    mvwprintw(window_registers, 2, 9, "SP[%u]", registers->SP);
    COLOR_OFF(window_registers);
    REG_CONDITIONAL_COLOR_ON(window_registers, registers->I_REG, REG16MID, REG16HIGH);
    mvwprintw(window_registers, 3, 9, "I[%u]", registers->I_REG);
    COLOR_OFF(window_registers);
    REG_CONDITIONAL_COLOR_ON(window_registers, registers->DT, REG8MID, REG8HIGH);
    mvwprintw(window_registers, 4, 9, "DT[%u]", registers->DT);
    COLOR_OFF(window_registers);
    REG_CONDITIONAL_COLOR_ON(window_registers, registers->ST, REG8MID, REG8HIGH);
    mvwprintw(window_registers, 5, 9, "ST[%u]", registers->ST);
    COLOR_OFF(window_registers);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            REG_CONDITIONAL_COLOR_ON(window_registers, registers->EMU_TEMP[i].bytes[j], REG8MID, REG8HIGH);
            mvwprintw(window_registers, 1 + (i * 3) + j, 17, "TEMP%u_BYTE%u[%u] ", i, j, registers->EMU_TEMP[i].bytes[j]);
            COLOR_OFF(window_registers);
        }

        REG_CONDITIONAL_COLOR_ON(window_registers, registers->EMU_TEMP[i].word, REG16MID, REG16HIGH);
        mvwprintw(window_registers, 1 + (i * 3) + 2, 17, "TEMP%u_WORD[%u] ", i, registers->EMU_TEMP[i].word);
        COLOR_OFF(window_registers);
    }

    wrefresh(window_registers);
    */
}

void render_emulator_state(EmulatorState_t *emu_state, WINDOW *window_emu)
{
	/*
    int8_t color_pair_idx = 0;

    werase(window_emu);
    box(window_emu, 0, 0);
    mvwprintw(window_emu, 1, 1, "Speed[%.2f]", emu_state->speed_modifier);
    mvwprintw(window_emu, 2, 1, "Step Counter[%u]", emu_state->step_counter);
    mvwprintw(window_emu, 3, 1, "Runtime[%.2fs]", emu_state->runtime_seconds_counter);
    mvwprintw(window_emu, 4, 1, "Avg. FPS[%.2f]", emu_state->avg_cps);

    for (int i = 0; i < CHIP8_KEY_COUNT; i++)
    {
        KEY_CONDITIONAL_COLOR_ON(window_emu, i);
        mvwprintw(window_emu, (i/4)+1, (8*(i%4))+24, "%s[%u] ", chip8_key_names[i], emu_state->chip8_key_states[i]);
        COLOR_OFF(window_emu);
    }

    //mvwprintw(window_emu, 5, 1, "Tick Counter[%u/%u]", tick_counter, tick_threshold);
    wrefresh(window_emu);
    */
}
