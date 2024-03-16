/*
 * File: \connection.h
 * Project: mpu6050
 * Created Date: 2024-03-06 23:41:26
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-16 15:38:39
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef MPU6050_CONNECTION_H
#define MPU6050_CONNECTION_H

#include <stdio.h>
#include "driver/i2c.h"

static const char *TAG = "i2c-mpu6050";

#define I2C_MASTER_SCL_IO GPIO_NUM_18 /*!< I2C时钟的GPIO口序号 */
#define I2C_MASTER_SDA_IO GPIO_NUM_19 /*!< I2C数据的GPIO口序号 */
#define I2C_MASTER_NUM 0              /*!< I2C主机的port号, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ 400000     /*!< I2C主机的时钟频率 */
#define I2C_MASTER_TX_BUF_DISABLE 0   /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0   /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS 1000

#define MPU6050_SENSOR_ADDR 0x68       /*!< MPU6050 的从机地址 */
#define MPU6050_WHO_AM_I_REG_ADDR 0x75 /*!< "who am I"的寄存器地址 */

#define MPU6050_PWR_MGMT_1_REG_ADDR 0x6B /*!< Register addresses of the power managment register */
#define MPU6050_RESET_BIT 7

/**
 * @brief 向MPU6050寄存器读取一个字节的数据
 */
static uint8_t MPU6050_register_read_byte(uint8_t reg_addr)
{
    uint8_t data;
    i2c_master_write_read_device(I2C_MASTER_NUM,
                                 MPU6050_SENSOR_ADDR,
                                 &reg_addr,
                                 1,
                                 &data,
                                 1,
                                 I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    return data;
}

/**
 * @brief 向MPU6050寄存器读取任意字节的数据
 */
static esp_err_t MPU6050_register_read_any(uint8_t reg_addr, uint8_t *data_buffer, size_t read_size)
{
    return i2c_master_write_read_device(I2C_MASTER_NUM,
                                 MPU6050_SENSOR_ADDR,
                                 &reg_addr,
                                 1,
                                 data_buffer,
                                 read_size,
                                 I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

/**
 * @brief 向MPU6050寄存器写入一个字节的数据
 */
static esp_err_t MPU6050_register_write_byte(uint8_t reg_addr, uint8_t data)
{
    int ret;
    uint8_t write_buf[2] = {reg_addr, data};

    ret = i2c_master_write_to_device(I2C_MASTER_NUM,
                                     MPU6050_SENSOR_ADDR,
                                     write_buf,
                                     sizeof(write_buf),
                                     I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    return ret;
}

/**
 * @brief 向MPU6050寄存器写入任意字节的数据
 */
static esp_err_t MPU6050_register_write_any(uint8_t reg_addr, uint8_t *data_buffer, size_t write_size)
{
    int ret;
    uint8_t *write_buf = pvPortMalloc(write_size + 1); // 分配足够的空间来存储寄存器地址和数据
    if (write_buf == NULL)
    {
        return ESP_ERR_NO_MEM; // 内存分配失败
    }
    write_buf[0] = reg_addr;
    memcpy(write_buf + 1, data_buffer, write_size); // 将数据复制到缓冲区中
    ret = i2c_master_write_to_device(I2C_MASTER_NUM,
                                     MPU6050_SENSOR_ADDR,
                                     write_buf,
                                     write_size + 1,
                                     I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    vPortFree(write_buf);

    return ret;
}

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

#endif
