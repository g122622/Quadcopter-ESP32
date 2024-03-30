/*
 * File: \controllerTick.h
 * Project: FlightController
 * Created Date: 2024-03-29 22:57:12
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-29 23:50:03
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#include "../mpu6050/motionData.h"
#include "../globalStates/motionState.h"
#include "../utils/F3D.h"

void controllerTick()
{
    /* 从全局变量读取用户指令 */

    /* 用户指令转为欧拉角和质心加速度 */
    float expectedPitch = 0;
    float expectedYaw = 0;
    float expectedAccel = 1;
    /* 读取传感器姿态数据 */
    AccelData = getAccelData();
    GyroData = getGyroData();
    F3D realRulerAngle = calcEulerAngle(AccelData, GyroData);

    /* 期望值和实际值作差 */
    float gyroErr = -GyroData.z;
}
