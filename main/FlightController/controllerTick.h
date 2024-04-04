/*
 * File: \controllerTick.h
 * Project: FlightController
 * Created Date: 2024-03-29 22:57:12
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-04 19:29:17
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#include "mpu6050/motionData.h"
#include "globalStates/motionState.h"
#include "utils/F3D.h"

#include "./PID/PerformPID.h"
#include "./PID/config/gyroPID.h"
#include "./PID/config/headingPID.h"
#include "./PID/config/throttlePID.h"

#include "FlightController/motor/motor.h"

void controllerTick()
{
    /* 从全局变量读取用户指令 */

    /* 用户指令转为欧拉角和质心加速度 */
    float expectedPitch = 0;
    float expectedRoll = 0;
    float expectedAccelMagnitude = 0.01;

    // 全局变量
    GyroData = getGyroData();
    AccelData = getAccelData();
    /* 读取传感器姿态数据 */
    float realAccelMagnitude = getAccelMagnitude();
    F3D realRulerAngle = calcEulerAngle(AccelData, GyroData);

    /* 期望值和实际值作差 */
    float pitchErr = expectedPitch - realRulerAngle.x;
    float rollErr = expectedRoll - realRulerAngle.y;
    float accelMagnitudeErr = expectedAccelMagnitude - realAccelMagnitude;
    float gyroErr = -GyroData.z;

    /* 执行PID算法 */
    float gyroPID = performPID(&gyroPIDConfig, gyroErr, 5);
    float rollPID = performPID(&rollPIDConfig, rollErr, 5);
    float pitchPID = performPID(&pitchPIDConfig, pitchErr, 5);

    /* 将PID输出值转为电机PWM百分比 */
    float mult = 1.5;
    float basic = 30;
    setMotorPWMPercentage(0, mult * (-rollPID + pitchPID) + basic);
    setMotorPWMPercentage(1, mult * (-rollPID - pitchPID) + basic);
    setMotorPWMPercentage(2, mult * (+rollPID - pitchPID) + basic);
    setMotorPWMPercentage(3, mult * (+rollPID + pitchPID) + basic);
}
