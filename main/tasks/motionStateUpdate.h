/*
 * File: \motion
 * Project: tasks
 * Created Date: 2024-03-11 13:27:21
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-21 17:13:50
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
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    while (1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        AccelData = getAccelData();
        GyroData = getGyroData();
        F3D res = calcEulerAngle(AccelData, GyroData);
        // printf("P: %f\tR: %f\tY: %f\n", res.x, res.y, res.z);
        // uint32_t time_stamp = esp_log_timestamp();
        // printf("timestamp = %ld \n", time_stamp);
        test(res.z);
    }
}

#endif
