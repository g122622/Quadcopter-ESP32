/*
 * File: \flashStatusLED.h
 * Project: LED
 * Created Date: 2024-04-28 15:07:32
 * Author: Guoyi
 * -----
 * Last Modified: 2024-05-08 23:53:27
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

static gpio_num_t gpio_led_num = GPIO_NUM_23; // 连接LED的GPIO
static int s_count = 0;
static int s_duration = 0;

static TaskHandle_t flashStatusLEDTaskHandle;
static SemaphoreHandle_t semapHandle;
void enableStatusLED();
void disableStatusLED();

static void flashStatusLEDTask(void *argument)
{
    while (1)
    {
        xSemaphoreTake(semapHandle, portMAX_DELAY);
        for (int i = 0; i < s_count; i++)
        {
            enableStatusLED();
            vTaskDelay(s_duration / portTICK_PERIOD_MS);
            disableStatusLED();
            vTaskDelay(s_duration / portTICK_PERIOD_MS);
        }
    }
}


void StatusLED_Init()
{
    // 设置控制LED的GPIO为输出模式
    gpio_set_direction(gpio_led_num, GPIO_MODE_OUTPUT);
    // 创建信号量，用于拉起LED频闪任务
    semapHandle = xSemaphoreCreateBinary();
    xTaskCreatePinnedToCore(flashStatusLEDTask, "flashStatusLEDTask",
                            1024, NULL, 3, flashStatusLEDTaskHandle, 0);
}

void enableStatusLED()
{
    gpio_set_level(gpio_led_num, 1);
}

void disableStatusLED()
{
    gpio_set_level(gpio_led_num, 0);
}

void flashStatusLED(int count, int duration)
{
    s_count = count;
    s_duration = duration;
    xSemaphoreGive(semapHandle); // 拉起信号量
}

#endif
