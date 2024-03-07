/*
 * File: \motionData.h
 * Project: mpu6050
 * Created Date: 2024-03-07 22:51:03
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-07 23:34:04
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#include "mpu6050.h"

#define ACCEL_RANGE 2
#define GYRO_RANGE 250

typedef struct
{
    double ax;
    double ay;
    double az;
} AccelData;

typedef struct
{
    double gx;
    double gy;
    double gz;
} GyroData;

void MotionData_Init()
{
    MPU6050_Init();
}

/**
 * 单位：g
*/
AccelData getAccelData()
{
    AccelData ret = {
        .ax = MPU6050_Get_16bit_Data(ACCEL_XOUT_H) / (double)(65536 / 2 / ACCEL_RANGE),
        .ay = MPU6050_Get_16bit_Data(ACCEL_YOUT_H) / (double)(65536 / 2 / ACCEL_RANGE),
        .az = MPU6050_Get_16bit_Data(ACCEL_ZOUT_H) / (double)(65536 / 2 / ACCEL_RANGE),
    };
    return ret;
}

/**
 * 单位：度/s
*/
GyroData getGyroData()
{
    GyroData ret = {
        .gx = MPU6050_Get_16bit_Data(GYRO_XOUT_H) / (double)(65536 / 2 / GYRO_RANGE),
        .gy = MPU6050_Get_16bit_Data(GYRO_YOUT_H) / (double)(65536 / 2 / GYRO_RANGE),
        .gz = MPU6050_Get_16bit_Data(GYRO_ZOUT_H) / (double)(65536 / 2 / GYRO_RANGE),
    };
    return ret;
}

void MotionData_Display()
{
    AccelData acc = getAccelData();
    /* 打印 x, y, z 轴加速度 */
    printf("ACCEL_X: %lf\t", acc.ax);
    printf("ACCEL_Y: %lf\t", acc.ay);
    printf("ACCEL_Z: %lf\t", acc.az);

    GyroData gyr = getGyroData();
    /* 打印 x, y, z 轴角速度 */
    printf("GYRO_X: %lf\t", gyr.gx);
    printf("GYRO_Y: %lf\t", gyr.gy);
    printf("GYRO_Z: %lf\t", gyr.gz);
}
