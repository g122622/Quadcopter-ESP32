/*
 * File: \motion
 * Project: tasks
 * Created Date: 2024-03-11 13:27:21
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-27 17:06:58
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

#include "mpu6050/motionData.h"

void controllerTickLoop(void *argument)
{
    MotionData_Init();
    vTaskDelay(200 / portTICK_PERIOD_MS);
    while (1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        controllerTick(10);
    }
}

#endif
