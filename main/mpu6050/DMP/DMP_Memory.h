/*
 * File: \DMP_Memory.h
 * Project: DMP
 * Created Date: 2024-03-16 14:30:22
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-16 21:43:34
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef DMP_MEMORY_H
#define DMP_MEMORY_H

#include "../connection.h"
#include "./DMP_Config.h"
#include "./DMP_Global.h"
#include "./dmpKey.h"

uint8_t dmp_on = 0;         // 状态
uint8_t int_enable = 0xff;  // 配置
uint8_t fifo_enable = 0xff; // 配置

/**
 *  @brief      Enable/disable data ready interrupt.
 *  If the DMP is on, the DMP interrupt is enabled. Otherwise, the data ready
 *  interrupt is used.
 *  @param[in]  enable      1 to enable interrupt.
 *  @return     0 if successful.
 */
static int
set_int_enable(unsigned char enable)
{
    unsigned char tmp;

    if (dmp_on)
    {
        if (enable)
            tmp = BIT_DMP_INT_EN;
        else
            tmp = 0x00;
        if (MPU6050_register_write_any(reg_table.int_enable, &tmp, 1))
            return -1;
        int_enable = tmp;
    }
    else
    {
        if (enable && int_enable)
            return 0;
        if (enable)
            tmp = BIT_DATA_RDY_EN;
        else
            tmp = 0x00;
        if (MPU6050_register_write_any(reg_table.int_enable, &tmp, 1))
            return -1;
        int_enable = tmp;
    }
    return 0;
}

/**
 *  @brief  Reset FIFO read/write pointers.
 *  @return 0 if successful.
 */
int mpu_reset_fifo(void)
{
    unsigned char data = 0;
    if (MPU6050_register_write_any(reg_table.int_enable, &data, 1))
        return -1;
    if (MPU6050_register_write_any(reg_table.fifo_en, &data, 1))
        return -1;
    if (MPU6050_register_write_any(reg_table.user_ctrl, &data, 1))
        return -1;

    if (dmp_on)
    {
        data = BIT_FIFO_RST | BIT_DMP_RST;
        if (MPU6050_register_write_any(reg_table.user_ctrl, &data, 1))
            return -1;

        data = BIT_DMP_EN | BIT_FIFO_EN;
        if (MPU6050_register_write_any(reg_table.user_ctrl, &data, 1))
            return -1;
        if (int_enable)
            data = BIT_DMP_INT_EN;
        else
            data = 0;
        if (MPU6050_register_write_any(reg_table.int_enable, &data, 1))
            return -1;
        data = 0;
        if (MPU6050_register_write_any(reg_table.fifo_en, &data, 1))
            return -1;
    }
    else
    {
        data = BIT_FIFO_RST;
        if (MPU6050_register_write_any(reg_table.user_ctrl, &data, 1))
            return -1;
        if (0xFF || !(0xFF & INV_XYZ_COMPASS))
            data = BIT_FIFO_EN;
        else
            data = BIT_FIFO_EN | BIT_AUX_IF_EN;
        if (MPU6050_register_write_any(reg_table.user_ctrl, &data, 1))
            return -1;

        if (int_enable)
            data = BIT_DATA_RDY_EN;
        else
            data = 0;
        if (MPU6050_register_write_any(reg_table.int_enable, &data, 1))
            return -1;
        if (MPU6050_register_write_any(reg_table.fifo_en, &fifo_enable, 1))
            return -1;
    }
    return 0;
}

/**
 *  @brief      Select which sensors are pushed to FIFO.
 *  @e sensors can contain a combination of the following flags:
 *  \n INV_X_GYRO, INV_Y_GYRO, INV_Z_GYRO
 *  \n INV_XYZ_GYRO
 *  \n INV_XYZ_ACCEL
 *  @param[in]  sensors Mask of sensors to push to FIFO.
 *  @return     0 if successful.
 */
int mpu_configure_fifo(unsigned char sensors)
{
    unsigned char prev;
    int result = 0;

    /* Compass data isn't going into the FIFO. Stop trying. */
    sensors &= ~INV_XYZ_COMPASS;

    prev = fifo_enable;
    fifo_enable = sensors & 0xff;
    if (fifo_enable != sensors)
        /* You're not getting what you asked for. Some sensors are
         * asleep.
         */
        result = -1;
    else
        result = 0;
    if (sensors || 0)
        set_int_enable(1);
    else
        set_int_enable(0);
    if (sensors)
    {
        if (mpu_reset_fifo())
        {
            fifo_enable = prev;
            return -1;
        }
    }

    return result;
}

/**
 *  @brief      Write to the DMP memory.
 *  会检查写入是否越界。
 *  DMP内存只在芯片awake状态才可访问。
 *  @param[in]  mem_addr    Memory location (bank << 8 | start address)
 *  @param[in]  length      Number of bytes to write.
 *  @param[in]  data        Bytes to write to memory.
 *  @return     0 if successful.
 */
int mpu_write_mem(unsigned short mem_addr, unsigned short length, unsigned char *data)
{
    unsigned char tmp[2];

    if (!data)
        return -1;

    tmp[0] = (unsigned char)(mem_addr >> 8);
    tmp[1] = (unsigned char)(mem_addr & 0xFF);

    /* 检查边界 */
    if (tmp[1] + length > hw.bank_size)
        return -1;

    if (MPU6050_register_write_any(reg_table.bank_sel, tmp, 2))
        return -1;
    if (MPU6050_register_write_any(reg_table.mem_r_w, data, length))
        return -1;
    return 0;
}

/**
 *  @brief      Read from the DMP memory.
 *  This function prevents I2C reads past the bank boundaries. The DMP memory
 *  is only accessible when the chip is awake.
 *  @param[in]  mem_addr    Memory location (bank << 8 | start address)
 *  @param[in]  length      Number of bytes to read.
 *  @param[out] data        Bytes read from memory.
 *  @return     0 if successful.
 */
int mpu_read_mem(unsigned short mem_addr, unsigned short length, unsigned char *data)
{
    unsigned char tmp[2];

    if (!data)
        return -1;

    tmp[0] = (unsigned char)(mem_addr >> 8);
    tmp[1] = (unsigned char)(mem_addr & 0xFF);

    /* Check bank boundaries. */
    if (tmp[1] + length > hw.bank_size)
        return -1;

    if (MPU6050_register_write_any(reg_table.bank_sel, tmp, 2))
        return -1;
    if (MPU6050_register_read_any(reg_table.mem_r_w, data, length))
        return -1;
    return 0;
}

/**
 *  @brief      Set DMP output rate.
 *  Only used when DMP is on.
 *  @param[in]  rate    Desired fifo rate (Hz).
 *  @return     0 if successful.
 */
int dmp_set_fifo_rate(unsigned short rate)
{
    const unsigned char regs_end[12] = {DINAFE, DINAF2, DINAAB,
                                        0xc4, DINAAA, DINAF1, DINADF, DINADF, 0xBB, 0xAF, DINADF, DINADF};
    unsigned short div;
    unsigned char tmp[8];

    if (rate > DMP_SAMPLE_RATE)
        return -1;
    div = DMP_SAMPLE_RATE / rate - 1;
    tmp[0] = (unsigned char)((div >> 8) & 0xFF);
    tmp[1] = (unsigned char)(div & 0xFF);
    if (mpu_write_mem(D_0_22, 2, tmp))
        return -1;
    if (mpu_write_mem(CFG_6, 12, (unsigned char *)regs_end))
        return -1;

    dmp.fifo_rate = rate;
    return 0;
}

/**
 *  @brief      Get one unparsed packet from the FIFO.
 *  This function should be used if the packet is to be parsed elsewhere.
 *  @param[in]  length  Length of one FIFO packet.
 *  @param[in]  data    FIFO packet.
 *  @param[in]  more    Number of remaining packets.
 */
int mpu_read_fifo_stream(unsigned short length, unsigned char *data,
                         unsigned char *more)
{
    unsigned char tmp[2];
    unsigned short fifo_count;
    if (!dmp_on)
        return -1;

    if (MPU6050_register_read_any(reg_table.fifo_count_h, tmp, 2))
        return -1;
    fifo_count = (tmp[0] << 8) | tmp[1];
    if (fifo_count < length)
    {
        more[0] = 0;
        return -1;
    }
    if (fifo_count > (hw.max_fifo >> 1))
    {
        /* FIFO is 50% full, better check overflow bit. */
        if (MPU6050_register_read_any(reg_table.int_status, tmp, 1))
            return -1;
        if (tmp[0] & BIT_FIFO_OVERFLOW)
        {
            mpu_reset_fifo();
            return -2;
        }
    }

    if (MPU6050_register_read_any(reg_table.fifo_r_w, data, length))
        return -1;
    more[0] = fifo_count / length - 1;
    return 0;
}

/**
 *  @brief      Get one packet from the FIFO.
 *  If @e sensors does not contain a particular sensor, disregard the data
 *  returned to that pointer.
 *  \n @e sensors can contain a combination of the following flags:
 *  \n INV_X_GYRO, INV_Y_GYRO, INV_Z_GYRO
 *  \n INV_XYZ_GYRO
 *  \n INV_XYZ_ACCEL
 *  \n INV_WXYZ_QUAT
 *  \n If the FIFO has no new data, @e sensors will be zero.
 *  \n If the FIFO is disabled, @e sensors will be zero and this function will
 *  return a non-zero error code.
 *  @param[out] gyro        Gyro data in hardware units.
 *  @param[out] accel       Accel data in hardware units.
 *  @param[out] quat        3-axis quaternion data in hardware units.
 *  @param[out] timestamp   Timestamp in milliseconds.
 *  @param[out] sensors     Mask of sensors read from FIFO.
 *  @param[out] more        Number of remaining packets.
 *  @return     0 if successful.
 */
int dmp_read_fifo(short *gyro, short *accel, long *quat, short *sensors, unsigned char *more)
{
    unsigned char fifo_data[MAX_PACKET_LENGTH];
    unsigned char ii = 0;
    sensors[0] = 0;

    /* Get a packet. */
    if (mpu_read_fifo_stream(dmp.packet_length, fifo_data, more))
        return -1;

    /* Parse DMP packet. */
    if (dmp.feature_mask & (DMP_FEATURE_LP_QUAT | DMP_FEATURE_6X_LP_QUAT))
    {
        quat[0] = ((long)fifo_data[0] << 24) | ((long)fifo_data[1] << 16) |
                  ((long)fifo_data[2] << 8) | fifo_data[3];
        quat[1] = ((long)fifo_data[4] << 24) | ((long)fifo_data[5] << 16) |
                  ((long)fifo_data[6] << 8) | fifo_data[7];
        quat[2] = ((long)fifo_data[8] << 24) | ((long)fifo_data[9] << 16) |
                  ((long)fifo_data[10] << 8) | fifo_data[11];
        quat[3] = ((long)fifo_data[12] << 24) | ((long)fifo_data[13] << 16) |
                  ((long)fifo_data[14] << 8) | fifo_data[15];
        ii += 16;
    }

    if (dmp.feature_mask & DMP_FEATURE_SEND_RAW_ACCEL)
    {
        accel[0] = ((short)fifo_data[ii + 0] << 8) | fifo_data[ii + 1];
        accel[1] = ((short)fifo_data[ii + 2] << 8) | fifo_data[ii + 3];
        accel[2] = ((short)fifo_data[ii + 4] << 8) | fifo_data[ii + 5];
        ii += 6;
        sensors[0] |= INV_XYZ_ACCEL;
    }

    if (dmp.feature_mask & DMP_FEATURE_SEND_ANY_GYRO)
    {
        gyro[0] = ((short)fifo_data[ii + 0] << 8) | fifo_data[ii + 1];
        gyro[1] = ((short)fifo_data[ii + 2] << 8) | fifo_data[ii + 3];
        gyro[2] = ((short)fifo_data[ii + 4] << 8) | fifo_data[ii + 5];
        ii += 6;
        sensors[0] |= INV_XYZ_GYRO;
    }

    return 0;
}

#endif