/*
 * screen.h
 *
 *  Created on: Mar 23, 2025
 *      Author: mickey
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <stdint.h>

#include "lcd_io.h"

void screen_fill_rect(uint8_t *data, uint16_t x_origin, uint16_t y_origin, uint16_t width, uint16_t height);
void screen_fill_rect_loop(uint8_t *data, uint16_t data_length, uint16_t x_origin, uint16_t y_origin, uint16_t width, uint16_t height);
uint32_t screen_get_x_size(void);
uint32_t screen_get_y_size(void);
void screen_await_transfer(void);

#endif /* SCREEN_H_ */
