/*
 * File: \motion
 * Project: tasks
 * Created Date: 2024-03-11 13:27:21
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-28 14:58:33
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
#include "../globalStates/controllerState.h"

#include "mpu6050/motionData.h"

void controllerTickLoop(void *argument)
{
    vTaskDelay(5000 / portTICK_PERIOD_MS); // 等待5000秒之后，开始初始化mpu6050，并进行校准
    MotionData_Init();
    vTaskDelay(200 / portTICK_PERIOD_MS);
    while (1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        switch (flightState)
        {
        case 0:
            stopAllMotors();
            break;

        case 1:
            controllerTick(10);
            break;

        default:
            break;
        }
    }
}

#endif
