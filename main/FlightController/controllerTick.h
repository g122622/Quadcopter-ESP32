/*
 * File: \controllerTick.h
 * Project: FlightController
 * Created Date: 2024-03-29 22:57:12
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-28 15:22:56
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

#include "./PID/PerformPID.h"
#include "./PID/config/gyroPID.h"
#include "./PID/config/headingPID.h"
#include "./PID/config/throttlePID.h"

#include "FlightController/motor/motor.h"
// #define PRINT_PWM_MODE

uint32_t tickCount = 0;

void controllerTick(int dt)
{
    /* 从全局变量读取用户指令 */

    /* 用户指令转为欧拉角和质心加速度 */
    float expectedPitch = 0;
    float expectedRoll = 0;
    float expectedAccelMagnitude = 0.01;

    // 更新全局变量
    GyroData = getGyroData();
    AccelData = getAccelData();
    /* 读取传感器姿态数据 */
    float realAccelMagnitude = getAccelMagnitude();
    F3D realRulerAngle = calcEulerAngle(AccelData, GyroData);
    // 更新全局变量
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
    float PWM1 = PWM_Mult * (-rollPID - pitchPID) + PWM_Basic;
    float PWM2 = PWM_Mult * (+rollPID - pitchPID) + PWM_Basic;
    float PWM3 = PWM_Mult * (+rollPID + pitchPID) + PWM_Basic;
    float PWM4 = PWM_Mult * (-rollPID + pitchPID) + PWM_Basic;
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
    // if (tickCount * dt >= 3000)
    // {
    //     stopAllMotors();
    //     esp_restart();
    // }
    setMotorPWMPercentage(0, PWM1);
    setMotorPWMPercentage(1, PWM2);
    setMotorPWMPercentage(2, PWM3);
    setMotorPWMPercentage(3, PWM4);
#endif
    tickCount++;
}
