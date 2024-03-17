/*
 * File: \DMP_Config.h
 * Project: DMP
 * Created Date: 2024-03-16 14:18:39
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-16 22:23:47
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */
#ifndef DMP_CONFIG_H
#define DMP_CONFIG_H

#define DMP_SAMPLE_RATE (500)
#define DEFAULT_MPU_HZ (10) // DMP输出频率
#define DMP_CODE_SIZE (3062)
#define MAX_PACKET_LENGTH (32)
/* 加载固件的时候需要使用。Must divide evenly into st.hw->bank_size to avoid bank crossings. */
#define LOAD_CHUNK (16)
#define GYRO_SF (46850825LL * 200 / DMP_SAMPLE_RATE)

// 方向信息
static signed char gyro_orientation[9] = {-1, 0, 0,
                                          0, -1, 0,
                                          0, 0, 1};

#endif