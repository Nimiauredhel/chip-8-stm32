/*
 * app.h
 *
 *  Created on: Mar 29, 2025
 *      Author: mickey
 */

#ifndef APP_H_
#define APP_H_

#include "main.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "gfx.h"
#include "utils.h"
#include "pitches.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim9;
extern UART_HandleTypeDef huart3;

void app_init(void);
void app_loop(void);
void app_clean(void);

#endif /* APP_H_ */
