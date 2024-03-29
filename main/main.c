/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* 蓝牙协议栈 */
#include "bluetooth/Bleprph_Init.h"
/* mpu6050 */
#include "mpu6050/motionData.h"
/* PWM */
#include "PWMDriver/motorPWM.h"
/* 各任务 */
#include "tasks/motionStateUpdate.h"

#define tskHIGH_PRIORITY 10

/* 启动任务 */
void Tasks_Init()
{
    TaskHandle_t motionStateUpdateHandle = NULL;
    xTaskCreatePinnedToCore(motionStateUpdate, "motionStateUpdate",
                            4096, NULL, tskHIGH_PRIORITY, motionStateUpdateHandle, 1);
}

void app_main(void)
{
    Bleprph_Init();
    MotionData_Init();
    MotorPWMDriver_Init();
    Tasks_Init();
}
