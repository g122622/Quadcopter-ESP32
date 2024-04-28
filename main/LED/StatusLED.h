/*
 * File: \flashStatusLED.h
 * Project: LED
 * Created Date: 2024-04-28 15:07:32
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-28 15:19:25
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef STATUS_LED_H
#define STATUS_LED_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

static gpio_num_t gpio_led_num = GPIO_NUM_32; // 连接LED的GPIO

void StatusLED_Init()
{
    // 设置控制LED的GPIO为输出模式
    gpio_set_direction(gpio_led_num, GPIO_MODE_OUTPUT);
}

void enableStatusLED()
{
    gpio_set_level(gpio_led_num, 1);
}

void disableStatusLED()
{
    gpio_set_level(gpio_led_num, 0);
}

#endif
