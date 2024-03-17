/*
 * File: \DMP.h
 * Project: DMP
 * Created Date: 2024-03-16 13:45:14
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-16 22:54:39
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef DMP_H
#define DMP_H

#include "./DMP_Firmware.h"
#include "./DMP_Orientation.h"
#include "./DMP_Global.h"
#include "./DMP_Feature.h"
#include "math.h"

int mpu_set_sample_rate(unsigned short rate);

/**
 *  @brief      Enable DMP support.
 *  @return     0 if successful.
 */
int mpu_enable_dmp()
{
    unsigned char tmp;
    /* Disable data ready interrupt. */
    set_int_enable(0);
    /* Disable bypass mode. */
    // mpu_set_bypass(0);
    /* Keep constant sample rate, FIFO rate controlled by DMP. */
    mpu_set_sample_rate(DMP_SAMPLE_RATE);
    /* Remove FIFO elements. */
    tmp = 0;
    MPU6050_register_write_any(0x23, &tmp, 1);
    dmp_on = 1;
    /* Enable DMP interrupt. */
    set_int_enable(1);
    mpu_reset_fifo();
    return 0;
}

/**
 *  @brief      Set sampling rate.
 *  Sampling rate must be between 4Hz and 1kHz.
 *  @param[in]  rate    Desired sampling rate (Hz).
 *  @return     0 if successful.
 */
int mpu_set_sample_rate(unsigned short rate)
{
    unsigned char data;

    if (rate < 4)
        rate = 4;
    else if (rate > 1000)
        rate = 1000;
    data = 1000 / rate - 1;

    if (MPU6050_register_write_any(reg_table.rate_div, &data, 1))
        return -1;

    // 原版在这里会设置低通滤波器，我取消了

    return 0;
}

/**************************************************************************
函数功能：MPU6050内置DMP的初始化
入口参数：无
返回  值：无
**************************************************************************/
void DMP_Init(void)
{
    if (!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
    {
        printf("1.mpu_configure_fifo complete ......\r\n");
    }
    if (!dmp_load_motion_driver_firmware())
    {
        printf("2.dmp_load_motion_driver_firmware complete ......\r\n");
    }
    if (!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))
    {
        printf("3.dmp_set_orientation complete ......\r\n");
    }
    if (!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO))
    {
        printf("4.dmp_enable_feature complete ......\r\n");
    }
    if (!dmp_set_fifo_rate(DEFAULT_MPU_HZ))
    {
        printf("5.dmp_set_fifo_rate complete ......\r\n");
    }
    if (!mpu_enable_dmp())
    {
        printf("6.mpu_enable_dmp complete ......\r\n");
    }
}

/**************************************************************************
函数功能：读取MPU6050内置DMP的姿态信息
入口参数：无
返回  值：无
**************************************************************************/
void DMP_Read(void)
{
    unsigned char more;
    long quat[4];
    short gyro[3], accel[3], sensors;
    float Pitch, Roll, Yaw;
    float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
#define q30 1073741824.0f

    dmp_read_fifo(gyro, accel, quat, &sensors, &more);
    q0 = quat[0] / q30;
    q1 = quat[1] / q30;
    q2 = quat[2] / q30;
    q3 = quat[3] / q30;
    Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;
    Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;     // roll
    Yaw = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3; // yaw
    printf("Pitch: %f ", Pitch);
    printf("Roll: %f ", Roll);
    printf("Yaw: %f \n", Yaw);
}

#endif