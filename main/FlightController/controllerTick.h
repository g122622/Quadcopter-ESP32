/*
 * File: \controllerTick.h
 * Project: FlightController
 * Created Date: 2024-03-29 22:57:12
 * Author: Guoyi
 * -----
 * Last Modified: 2024-05-03 00:10:13
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#include "mpu6050/motionData.h"
#include "globalStates/motionState.h"
#include "globalStates/PWMState.h"
#include "utils/F3D.h"
#include "utils/MathUtils.h"

#include "./PID/PerformPID.h"
#include "./PID/config/gyroPID.h"
#include "./PID/config/headingPID.h"
#include "./PID/config/throttlePID.h"

#include "FlightController/motor/motor.h"

#define BOUNCE_TIME_MS 1000

// #define PRINT_PWM_MODE

uint32_t tickCount = 0;

void controllerTick(int dt, bool shouldDriveMotors)
{
    /* 从全局变量读取用户指令 */

    /* 用户指令转为欧拉角和质心加速度 */
    float expectedPitch = 0;
    float expectedRoll = 0;
    float expectedAccelMagnitude = 0.01;

    // 更新全局变量 1
    GyroData = getGyroData();
    AccelData = getAccelData();
    /* 读取传感器姿态数据 */
    float realAccelMagnitude = getAccelMagnitude();
    F3D realRulerAngle = calcEulerAngle(AccelData, GyroData);
    // 更新全局变量 2
    EulerAngleData = realRulerAngle;

    /* 期望值和实际值作差 */
    float pitchErr = expectedPitch - realRulerAngle.x;
    float rollErr = expectedRoll - realRulerAngle.y;
    float accelMagnitudeErr = expectedAccelMagnitude - realAccelMagnitude;
    float gyroErr = -GyroData.z;

    /* 执行PID算法 */
    float gyroPID = performPID(&gyroPIDConfig, gyroErr, dt);
    float rollPID = performPID(&rollPIDConfig, rollErr, dt);
    float pitchPID = performPID(&pitchPIDConfig, pitchErr, dt);

    /* 将PID输出值转为电机PWM百分比 */
    float PWM1 = PID_Mult * (-rollPID - pitchPID) + MIN(PWM_Basic, PWM_Basic / BOUNCE_TIME_MS * (tickCount * dt));
    float PWM2 = PID_Mult * (+rollPID - pitchPID) + MIN(PWM_Basic, PWM_Basic / BOUNCE_TIME_MS * (tickCount * dt));
    float PWM3 = PID_Mult * (+rollPID + pitchPID) + MIN(PWM_Basic, PWM_Basic / BOUNCE_TIME_MS * (tickCount * dt));
    float PWM4 = PID_Mult * (-rollPID + pitchPID) + MIN(PWM_Basic, PWM_Basic / BOUNCE_TIME_MS * (tickCount * dt));
    PWM1 *= PWM1_Mult;
    PWM2 *= PWM2_Mult;
    PWM3 *= PWM3_Mult;
    PWM4 *= PWM4_Mult;
#ifdef PRINT_PWM_MODE
    if ((tickCount % 100) == 0)
    {

        printf("m%d, PWM: %f \t", 1, PWM1);
        printf("m%d, PWM: %f \t", 2, PWM2);
        printf("m%d, PWM: %f \t", 3, PWM3);
        printf("m%d, PWM: %f \n", 4, PWM4);
    }
#endif
#ifndef PRINT_PWM_MODE
    if (shouldDriveMotors)
    {
        if (tickCount % 2) // 每隔两个tick，更新一次电机PWM
        {
            setMotorPWMPercentage(0, PWM1);
            setMotorPWMPercentage(1, PWM2);
            setMotorPWMPercentage(2, PWM3);
            setMotorPWMPercentage(3, PWM4);
        }
    }
    else
    {
        stopAllMotors();
    }
#endif
    tickCount++;
}
