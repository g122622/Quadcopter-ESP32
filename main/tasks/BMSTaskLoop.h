/*
 * File: \BMSTask.h
 * Project: tasks
 * Created Date: 2024-04-24 17:28:06
 * Author: Guoyi
 * -----
 * Last Modified: 2024-05-15 17:04:06
 * Modified By:
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef TASKS_BMS_H
#define TASKS_BMS_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "BMS/BMSTick.h"

void BMSTaskLoop(void *argument)
{
    static uint32_t tickCount = 0;
    
    BMS_ADC_Init();
    while (1)
    {
        vTaskDelay(50 / portTICK_PERIOD_MS);
        BMSTick(tickCount);
        tickCount++;
    }
}

#endif
