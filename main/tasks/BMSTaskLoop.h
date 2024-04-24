/*
 * File: \BMSTask.h
 * Project: tasks
 * Created Date: 2024-04-24 17:28:06
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-24 17:31:36
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

#include "../BMS/ADC/batteryVoltage.h"

void BMSTaskLoop(void *argument)
{
    BMS_ADC_Init();
    while (1)
    {
        BMS_ADC_ReadBatteryVoltage();
        int volt = BMS_ADC_ReadBatteryVoltage();
        printf("volt=%d", volt);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

#endif
