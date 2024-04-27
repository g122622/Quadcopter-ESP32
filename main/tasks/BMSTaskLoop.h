/*
 * File: \BMSTask.h
 * Project: tasks
 * Created Date: 2024-04-24 17:28:06
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-27 13:00:59
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
#include "../globalStates/BMSState.h"

void BMSTaskLoop(void *argument)
{
    BMS_ADC_Init();
    while (1)
    {
        batteryVoltage = BMS_ADC_ReadBatteryVoltage();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

#endif
