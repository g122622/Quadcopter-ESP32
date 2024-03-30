/*
 * File: \motion
 * Project: tasks
 * Created Date: 2024-03-11 13:27:21
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-29 22:59:07
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef TASKS_MSU_H
#define TASKS_MSU_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../FlightController/controllerTick.h"

#define INTERVAL_MS (10)

void controllerTickLoop(void *argument)
{
    while (1)
    {
        vTaskDelay(INTERVAL_MS / portTICK_PERIOD_MS);
        controllerTick();
    }
}

#endif
