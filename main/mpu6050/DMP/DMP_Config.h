/*
 * File: \DMP_Config.h
 * Project: DMP
 * Created Date: 2024-03-16 14:18:39
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-16 16:06:57
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 * 
 * ------------------------------------
 */

#define DMP_SAMPLE_RATE (200)
#define DMP_CODE_SIZE (3062)
/* 加载固件的时候需要使用。Must divide evenly into st.hw->bank_size to avoid bank crossings. */
#define LOAD_CHUNK (16)
#define GYRO_SF (46850825LL * 200 / DMP_SAMPLE_RATE)

// 方向信息
static signed char gyro_orientation[9] = {-1, 0, 0,
                                          0, -1, 0,
                                          0, 0, 1};
