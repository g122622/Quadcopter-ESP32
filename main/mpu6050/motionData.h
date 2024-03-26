/*
 * File: \motionData.h
 * Project: mpu6050
 * Created Date: 2024-03-07 22:51:03
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-21 15:36:36
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef MOTION_DATA_H
#define MOTION_DATA_H

#include "mpu6050.h"
#include "../utils/F3D.h"
#include <math.h>

#define ACCEL_RANGE 2
#define GYRO_RANGE 250
/* 欧拉角解算相关的配置 */
#define Kp 100.0f    // 比例增益支配率收敛到加速度计/磁强计
#define Ki 0.002f    // 积分增益支配率的陀螺仪偏见的衔接
#define halfT 0.005f // 采样周期的一半

static float q0 = 1, q1 = 0, q2 = 0, q3 = 0;  // 四元数的元素，代表估计方向
static float exInt = 0, eyInt = 0, ezInt = 0; // 按比例缩小积分误差

void MotionData_Init()
{
    MPU6050_Init();
}

/**
 * 单位：g
 */
F3D getAccelData()
{
    F3D ret = {
        .x = MPU6050_Get_16bit_Data(ACCEL_XOUT_H) / (double)(65536 / 2 / ACCEL_RANGE),
        .y = MPU6050_Get_16bit_Data(ACCEL_YOUT_H) / (double)(65536 / 2 / ACCEL_RANGE),
        .z = MPU6050_Get_16bit_Data(ACCEL_ZOUT_H) / (double)(65536 / 2 / ACCEL_RANGE),
    };
    return ret;
}

/**
 * 单位：度/s
 */
F3D getGyroData()
{
    F3D ret = {
        .x = MPU6050_Get_16bit_Data(GYRO_XOUT_H) / (double)(65536 / 2 / GYRO_RANGE),
        .y = MPU6050_Get_16bit_Data(GYRO_YOUT_H) / (double)(65536 / 2 / GYRO_RANGE),
        .z = MPU6050_Get_16bit_Data(GYRO_ZOUT_H) / (double)(65536 / 2 / GYRO_RANGE),
    };
    return ret;
}

void test(float yaw){
    static float last_yaw = 0;
    printf("delta_yaw = %f \n", yaw - last_yaw);
    last_yaw = yaw;
}

F3D calcEulerAngle(F3D accel, F3D gyro)
{
    float ax = accel.x, ay = accel.y, az = accel.z;
    float gx = gyro.x, gy = gyro.y, gz = gyro.z;
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;

    // 测量正常化
    norm = sqrt(ax * ax + ay * ay + az * az);
    ax = ax / norm; // 单位化
    ay = ay / norm;
    az = az / norm;

    // 估计方向的重力
    vx = 2 * (q1 * q3 - q0 * q2);
    vy = 2 * (q0 * q1 + q2 * q3);
    vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

    // 错误的领域和方向传感器测量参考方向之间的交叉乘积的总和
    ex = (ay * vz - az * vy);
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);

    // 积分误差比例积分增益
    exInt = exInt + ex * Ki;
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

    // 调整后的陀螺仪测量
    gx = gx + Kp * ex + exInt;
    gy = gy + Kp * ey + eyInt;
    gz = gz + Kp * ez + ezInt;

    // 整合四元数率和正常化
    q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
    q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
    q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
    q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;

    // 正常化四元
    norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    float Pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;                                // pitch ,转换为度数
    float Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3; // rollv
    // float Yaw = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3;
    float Yaw = atan2(2 * (q1 * q2 + q0 * q3), 1 - 2 * (q2 * q2 + q3 * q3)) * 57.3;
    F3D ret;
    ret.x = Pitch;
    ret.y = Roll;
    ret.z = Yaw;
    return ret;
}

/**
 * 打印运动数据。仅供调试使用。
 */
void MotionData_Display()
{
    F3D acc = getAccelData();
    /* 打印 x, y, z 轴加速度 */
    printf("ACCEL_X: %lf\t", acc.x);
    printf("ACCEL_Y: %lf\t", acc.y);
    printf("ACCEL_Z: %lf\t", acc.z);

    F3D gyr = getGyroData();
    /* 打印 x, y, z 轴角速度 */
    printf("GYRO_X: %lf\t", gyr.x);
    printf("GYRO_Y: %lf\t", gyr.y);
    printf("GYRO_Z: %lf\t", gyr.z);
}

#endif
