/*
 * File: \motionData.h
 * Project: mpu6050
 * Created Date: 2024-03-07 22:51:03
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-12 13:35:41
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

#define ACCEL_RANGE 2
#define GYRO_RANGE 250

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
