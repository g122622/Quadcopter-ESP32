/*
 * File: \DMP_Memory.h
 * Project: DMP
 * Created Date: 2024-03-16 14:30:22
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-16 16:02:27
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#include "../connection.h"
#include "./DMP_Config.h"
#include "./DMP_Global.h"

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