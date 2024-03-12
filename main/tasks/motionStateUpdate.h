/*
 * File: \motion
 * Project: tasks
 * Created Date: 2024-03-11 13:27:21
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-12 13:35:41
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
#include "../mpu6050/motionData.h"
#include "../globalStates/motionState.h"
#include "../utils/F3D.h"

void motionStateUpdate(void *argument)
{
    while (1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        AccelData = getAccelData();
        GyroData = getGyroData();
    }
}

#endif
