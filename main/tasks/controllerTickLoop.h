/*
 * File: \motion
 * Project: tasks
 * Created Date: 2024-03-11 13:27:21
 * Author: Guoyi
 * -----
 * Last Modified: 2024-05-08 16:33:14
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

#define CONTROLLER_TICK_DURATION 5

void controllerTickLoop(void *argument)
{
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 等待1秒之后，开始初始化mpu6050，并进行校准
    MotionData_Init();
    vTaskDelay(200 / portTICK_PERIOD_MS);
    while (1)
    {
        vTaskDelay(CONTROLLER_TICK_DURATION / portTICK_PERIOD_MS);
        switch (flightState)
        {
        case 0: // 电机停车，其他流程正常执行
            controllerTick(CONTROLLER_TICK_DURATION, false);
            break;

        case 1: // 系统所有逻辑正常运行
            controllerTick(CONTROLLER_TICK_DURATION, true);
            break;

        default:
            break;
        }
    }
}

#endif
