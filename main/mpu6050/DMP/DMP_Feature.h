/*
 * File: \DMP_Feature.h
 * Project: DMP
 * Created Date: 2024-03-16 16:05:40
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-16 16:10:21
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 * 
 * ------------------------------------
 */

#include "./DMP_Global.h"
#include "./DMP_Config.h"
#include "./DMP_Memory.h"
#include "./dmpKey.h"

/**
 *  @brief      Calibrate the gyro data in the DMP.
 *  After eight seconds of no motion, the DMP will compute gyro biases and
 *  subtract them from the quaternion output. If @e dmp_enable_feature is
 *  called with @e DMP_FEATURE_SEND_CAL_GYRO, the biases will also be
 *  subtracted from the gyro output.
 *  @param[in]  enable  1 to enable gyro calibration.
 *  @return     0 if successful.
 */
int dmp_enable_gyro_cal(unsigned char enable)
{
    if (enable)
    {
        unsigned char regs[9] = {0xb8, 0xaa, 0xb3, 0x8d, 0xb4, 0x98, 0x0d, 0x35, 0x5d};
        return mpu_write_mem(CFG_MOTION_BIAS, 9, regs);
    }
    else
    {
        unsigned char regs[9] = {0xb8, 0xaa, 0xaa, 0xaa, 0xb0, 0x88, 0xc3, 0xc5, 0xc7};
        return mpu_write_mem(CFG_MOTION_BIAS, 9, regs);
    }
}

/**
 *  @brief      Set tap threshold for a specific axis.
 *  @param[in]  axis    1, 2, and 4 for XYZ accel, respectively.
 *  @param[in]  thresh  Tap threshold, in mg/ms.
 *  @return     0 if successful.
 */
int dmp_set_tap_thresh(unsigned char axis, unsigned short thresh)
{
    unsigned char tmp[4], accel_fsr;
    float scaled_thresh;
    unsigned short dmp_thresh, dmp_thresh_2;
    if (!(axis & TAP_XYZ) || thresh > 1600)
        return -1;

    scaled_thresh = (float)thresh / DMP_SAMPLE_RATE;

    mpu_get_accel_fsr(&accel_fsr);
    switch (accel_fsr)
    {
    case 2:
        dmp_thresh = (unsigned short)(scaled_thresh * 16384);
        /* dmp_thresh * 0.75 */
        dmp_thresh_2 = (unsigned short)(scaled_thresh * 12288);
        break;
    case 4:
        dmp_thresh = (unsigned short)(scaled_thresh * 8192);
        /* dmp_thresh * 0.75 */
        dmp_thresh_2 = (unsigned short)(scaled_thresh * 6144);
        break;
    case 8:
        dmp_thresh = (unsigned short)(scaled_thresh * 4096);
        /* dmp_thresh * 0.75 */
        dmp_thresh_2 = (unsigned short)(scaled_thresh * 3072);
        break;
    case 16:
        dmp_thresh = (unsigned short)(scaled_thresh * 2048);
        /* dmp_thresh * 0.75 */
        dmp_thresh_2 = (unsigned short)(scaled_thresh * 1536);
        break;
    default:
        return -1;
    }
    tmp[0] = (unsigned char)(dmp_thresh >> 8);
    tmp[1] = (unsigned char)(dmp_thresh & 0xFF);
    tmp[2] = (unsigned char)(dmp_thresh_2 >> 8);
    tmp[3] = (unsigned char)(dmp_thresh_2 & 0xFF);

    if (axis & TAP_X)
    {
        if (mpu_write_mem(DMP_TAP_THX, 2, tmp))
            return -1;
        if (mpu_write_mem(D_1_36, 2, tmp + 2))
            return -1;
    }
    if (axis & TAP_Y)
    {
        if (mpu_write_mem(DMP_TAP_THY, 2, tmp))
            return -1;
        if (mpu_write_mem(D_1_40, 2, tmp + 2))
            return -1;
    }
    if (axis & TAP_Z)
    {
        if (mpu_write_mem(DMP_TAP_THZ, 2, tmp))
            return -1;
        if (mpu_write_mem(D_1_44, 2, tmp + 2))
            return -1;
    }
    return 0;
}

/**
 *  @brief      Set which axes will register a tap.
 *  @param[in]  axis    1, 2, and 4 for XYZ, respectively.
 *  @return     0 if successful.
 */
int dmp_set_tap_axes(unsigned char axis)
{
    unsigned char tmp = 0;

    if (axis & TAP_X)
        tmp |= 0x30;
    if (axis & TAP_Y)
        tmp |= 0x0C;
    if (axis & TAP_Z)
        tmp |= 0x03;
    return mpu_write_mem(D_1_72, 1, &tmp);
}

/**
 *  @brief      Set minimum number of taps needed for an interrupt.
 *  @param[in]  min_taps    Minimum consecutive taps (1-4).
 *  @return     0 if successful.
 */
int dmp_set_tap_count(unsigned char min_taps)
{
    unsigned char tmp;

    if (min_taps < 1)
        min_taps = 1;
    else if (min_taps > 4)
        min_taps = 4;

    tmp = min_taps - 1;
    return mpu_write_mem(D_1_79, 1, &tmp);
}

/**
 *  @brief      Set length between valid taps.
 *  @param[in]  time    Milliseconds between taps.
 *  @return     0 if successful.
 */
int dmp_set_tap_time(unsigned short time)
{
    unsigned short dmp_time;
    unsigned char tmp[2];

    dmp_time = time / (1000 / DMP_SAMPLE_RATE);
    tmp[0] = (unsigned char)(dmp_time >> 8);
    tmp[1] = (unsigned char)(dmp_time & 0xFF);
    return mpu_write_mem(DMP_TAPW_MIN, 2, tmp);
}

/**
 *  @brief      Set max time between taps to register as a multi-tap.
 *  @param[in]  time    Max milliseconds between taps.
 *  @return     0 if successful.
 */
int dmp_set_tap_time_multi(unsigned short time)
{
    unsigned short dmp_time;
    unsigned char tmp[2];

    dmp_time = time / (1000 / DMP_SAMPLE_RATE);
    tmp[0] = (unsigned char)(dmp_time >> 8);
    tmp[1] = (unsigned char)(dmp_time & 0xFF);
    return mpu_write_mem(D_1_218, 2, tmp);
}

/**
 *  @brief      Enable DMP features.
 *  The following \#define's are used in the input mask:
 *  \n DMP_FEATURE_TAP
 *  \n DMP_FEATURE_ANDROID_ORIENT
 *  \n DMP_FEATURE_LP_QUAT
 *  \n DMP_FEATURE_6X_LP_QUAT
 *  \n DMP_FEATURE_GYRO_CAL
 *  \n DMP_FEATURE_SEND_RAW_ACCEL
 *  \n DMP_FEATURE_SEND_RAW_GYRO
 *  \n NOTE: DMP_FEATURE_LP_QUAT and DMP_FEATURE_6X_LP_QUAT are mutually
 *  exclusive.
 *  \n NOTE: DMP_FEATURE_SEND_RAW_GYRO and DMP_FEATURE_SEND_CAL_GYRO are also
 *  mutually exclusive.
 *  @param[in]  mask    Mask of features to enable.
 *  @return     0 if successful.
 */
int dmp_enable_feature(unsigned short mask)
{
    unsigned char tmp[10];

    /* TODO: All of these settings can probably be integrated into the default
     * DMP image.
     */
    /* Set integration scale factor. */
    tmp[0] = (unsigned char)((GYRO_SF >> 24) & 0xFF);
    tmp[1] = (unsigned char)((GYRO_SF >> 16) & 0xFF);
    tmp[2] = (unsigned char)((GYRO_SF >> 8) & 0xFF);
    tmp[3] = (unsigned char)(GYRO_SF & 0xFF);
    mpu_write_mem(D_0_104, 4, tmp);

    /* Send sensor data to the FIFO. */
    tmp[0] = 0xA3;
    if (mask & DMP_FEATURE_SEND_RAW_ACCEL)
    {
        tmp[1] = 0xC0;
        tmp[2] = 0xC8;
        tmp[3] = 0xC2;
    }
    else
    {
        tmp[1] = 0xA3;
        tmp[2] = 0xA3;
        tmp[3] = 0xA3;
    }
    if (mask & DMP_FEATURE_SEND_ANY_GYRO)
    {
        tmp[4] = 0xC4;
        tmp[5] = 0xCC;
        tmp[6] = 0xC6;
    }
    else
    {
        tmp[4] = 0xA3;
        tmp[5] = 0xA3;
        tmp[6] = 0xA3;
    }
    tmp[7] = 0xA3;
    tmp[8] = 0xA3;
    tmp[9] = 0xA3;
    mpu_write_mem(CFG_15, 10, tmp);

    /* Send gesture data to the FIFO. */
    if (mask & (DMP_FEATURE_TAP | DMP_FEATURE_ANDROID_ORIENT))
        tmp[0] = DINA20;
    else
        tmp[0] = 0xD8;
    mpu_write_mem(CFG_27, 1, tmp);

    if (mask & DMP_FEATURE_GYRO_CAL)
        dmp_enable_gyro_cal(1);
    else
        dmp_enable_gyro_cal(0);

    if (mask & DMP_FEATURE_SEND_ANY_GYRO)
    {
        if (mask & DMP_FEATURE_SEND_CAL_GYRO)
        {
            tmp[0] = 0xB2;
            tmp[1] = 0x8B;
            tmp[2] = 0xB6;
            tmp[3] = 0x9B;
        }
        else
        {
            tmp[0] = DINAC0;
            tmp[1] = DINA80;
            tmp[2] = DINAC2;
            tmp[3] = DINA90;
        }
        mpu_write_mem(CFG_GYRO_RAW_DATA, 4, tmp);
    }

    if (mask & DMP_FEATURE_TAP)
    {
        /* Enable tap. */
        tmp[0] = 0xF8;
        mpu_write_mem(CFG_20, 1, tmp);
        dmp_set_tap_thresh(TAP_XYZ, 250);
        dmp_set_tap_axes(TAP_XYZ);
        dmp_set_tap_count(1);
        dmp_set_tap_time(100);
        dmp_set_tap_time_multi(500);

        dmp_set_shake_reject_thresh(GYRO_SF, 200);
        dmp_set_shake_reject_time(40);
        dmp_set_shake_reject_timeout(10);
    }
    else
    {
        tmp[0] = 0xD8;
        mpu_write_mem(CFG_20, 1, tmp);
    }

    if (mask & DMP_FEATURE_ANDROID_ORIENT)
    {
        tmp[0] = 0xD9;
    }
    else
        tmp[0] = 0xD8;
    mpu_write_mem(CFG_ANDROID_ORIENT_INT, 1, tmp);

    if (mask & DMP_FEATURE_LP_QUAT)
        dmp_enable_lp_quat(1);
    else
        dmp_enable_lp_quat(0);

    if (mask & DMP_FEATURE_6X_LP_QUAT)
        dmp_enable_6x_lp_quat(1);
    else
        dmp_enable_6x_lp_quat(0);

    /* Pedometer is always enabled. */
    dmp.feature_mask = mask | DMP_FEATURE_PEDOMETER;
    mpu_reset_fifo();

    dmp.packet_length = 0;
    if (mask & DMP_FEATURE_SEND_RAW_ACCEL)
        dmp.packet_length += 6;
    if (mask & DMP_FEATURE_SEND_ANY_GYRO)
        dmp.packet_length += 6;
    if (mask & (DMP_FEATURE_LP_QUAT | DMP_FEATURE_6X_LP_QUAT))
        dmp.packet_length += 16;
    if (mask & (DMP_FEATURE_TAP | DMP_FEATURE_ANDROID_ORIENT))
        dmp.packet_length += 4;

    return 0;
}
