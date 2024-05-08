/*
 * File: \mpu6050.h
 * Project: mpu6050
 * Created Date: 2024-03-07 19:55:56
 * Author: Guoyi
 * -----
 * Last Modified: 2024-05-08 13:42:51
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef MPU6050_H
#define MPU6050_H

#include "connection.h"
#include "esp_log.h"

//-----------------------------------------
// 定义MPU6050内部地址
//-----------------------------------------
#define SMPLRT_DIV 0x19   // 陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIG 0x1A       // 低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG 0x1B  // 陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG 0x1C // 加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

/* 加速度相关寄存器地址 */
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

/* 温度相关寄存器地址 */
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

/* 陀螺仪相关寄存器地址 */
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define PWR_MGMT_1 0x6B // 电源管理，典型值：0x00(正常启用)

void MPU6050_Init()
{
    ESP_ERROR_CHECK(i2c_master_init());
    /* 读取WHO_AM_I寄存器，确保i2c通信正常*/
    MPU6050_register_read_byte(MPU6050_WHO_AM_I_REG_ADDR);
    ESP_LOGI(TAG, "MPU6050建立连接成功");

    uint8_t flag = ESP_OK;
    flag = MPU6050_register_write_byte(PWR_MGMT_1, 0x00);   // 解除休眠状态
    flag = MPU6050_register_write_byte(SMPLRT_DIV, 0x00);   // 陀螺仪采样率。1KHz
    flag = MPU6050_register_write_byte(CONFIG, 0x06);       // 启用低通滤波。等级为0x05
    flag = MPU6050_register_write_byte(GYRO_CONFIG, 0x0);   // 陀螺仪自检及测量范围。不自检，250deg/s
    flag = MPU6050_register_write_byte(ACCEL_CONFIG, 0x01); // 加速计自检、测量范围及高通滤波频率。不自检，2G，5Hz
    if (flag != ESP_OK)
    {
        ESP_LOGE(TAG, "MPU6050发送初始化指令失败");
    }
    else
    {
        ESP_LOGI(TAG, "MPU6050发送初始化指令成功");
    }
}

int16_t MPU6050_Get_16bit_Data(uint8_t regAddr)
{
    uint8_t Data_H, Data_L;
    uint16_t data;

    Data_H = MPU6050_register_read_byte(regAddr);
    Data_L = MPU6050_register_read_byte(regAddr + 1);
    data = (Data_H << 8) | Data_L; // 合成数据
    return data;
}

void MPU6050_Display(void)
{
    /* 打印 x, y, z 轴加速度 */
    printf("ACCEL_X: %d\t", MPU6050_Get_16bit_Data(ACCEL_XOUT_H));
    printf("ACCEL_Y: %d\t", MPU6050_Get_16bit_Data(ACCEL_YOUT_H));
    printf("ACCEL_Z: %d\t", MPU6050_Get_16bit_Data(ACCEL_ZOUT_H));

    /* 打印温度，需要根据手册的公式换算为摄氏度 */
    printf("TEMP: %0.2f\t", MPU6050_Get_16bit_Data(TEMP_OUT_H) / 340.0 + 36.53);

    /* 打印 x, y, z 轴角速度 */
    printf("GYRO_X: %d\t", MPU6050_Get_16bit_Data(GYRO_XOUT_H));
    printf("GYRO_Y: %d\t", MPU6050_Get_16bit_Data(GYRO_YOUT_H));
    printf("GYRO_Z: %d\t", MPU6050_Get_16bit_Data(GYRO_ZOUT_H));

    printf("\r\n");
}

#endif
