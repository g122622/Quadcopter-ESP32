/*
 * File: \DMP.h
 * Project: DMP
 * Created Date: 2024-03-16 13:45:14
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-16 16:03:28
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#include "./DMP_Firmware.h"
#include "./DMP_Orientation.h"
#include "./DMP_Global.h"

/**************************************************************************
函数功能：MPU6050内置DMP的初始化
入口参数：无
返回  值：无
**************************************************************************/
void DMP_Init(void)
{
    if (!dmp_load_motion_driver_firmware())
    {
        // printf("dmp_load_motion_driver_firmware complete ......\r\n");
    }
    if (!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))
    {
        // printf("dmp_set_orientation complete ......\r\n");
    }
    if (!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
                            DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
                            DMP_FEATURE_GYRO_CAL))
    {
        // printf("dmp_enable_feature complete ......\r\n");
    }
    if (!dmp_set_fifo_rate(DEFAULT_MPU_HZ))
    {
        // printf("dmp_set_fifo_rate complete ......\r\n");
    }
    run_self_test();
    if (!mpu_set_dmp_state(1))
    {
        // printf("mpu_set_dmp_state complete ......\r\n");
    }
}

/**************************************************************************
函数功能：读取MPU6050内置DMP的姿态信息
入口参数：无
返回  值：无
**************************************************************************/
void Read_DMP(void)
{
    unsigned long sensor_timestamp;
    unsigned char more;
    long quat[4];

    dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more);
    if (sensors & INV_WXYZ_QUAT)
    {
        q0 = quat[0] / q30;
        q1 = quat[1] / q30;
        q2 = quat[2] / q30;
        q3 = quat[3] / q30;
        //		 Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;
        Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;     // roll
        Yaw = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3; // yaw
    }
}
