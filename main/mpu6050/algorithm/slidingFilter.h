/*
 * File: \slidingFilter.h
 * Project: algorithm
 * Created Date: 2024-04-29 21:10:08
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-29 22:54:36
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef SLIDING_FILTER_H
#define SLIDING_FILTER_H

#include <stdio.h>
#include "utils/F3D.h"

#define SLIDING_FILTER_QUEUE_LEN 10

// 只对加速度进行滑动滤波
static float SlidingFilterQueue[3][SLIDING_FILTER_QUEUE_LEN]; // gcc默认初始化全0，不用手动置为零
static int insertStart = 0;
static float sumX = 0;
static float sumY = 0;
static float sumZ = 0;

F3D performSlidingFilter(F3D OriginalData)
{
    float ax = OriginalData.x;
    float ay = OriginalData.y;
    float az = OriginalData.z;
    
    // 更新sum
    sumX += ax / SLIDING_FILTER_QUEUE_LEN - SlidingFilterQueue[0][insertStart];
    sumY += ay / SLIDING_FILTER_QUEUE_LEN - SlidingFilterQueue[1][insertStart];
    sumZ += az / SLIDING_FILTER_QUEUE_LEN - SlidingFilterQueue[2][insertStart];

    // 更新队列
    SlidingFilterQueue[0][insertStart] = ax / SLIDING_FILTER_QUEUE_LEN;
    SlidingFilterQueue[1][insertStart] = ay / SLIDING_FILTER_QUEUE_LEN;
    SlidingFilterQueue[2][insertStart] = az / SLIDING_FILTER_QUEUE_LEN;
    insertStart++;
    if (insertStart >= SLIDING_FILTER_QUEUE_LEN)
    {
        insertStart = 0;
    }

    // 包装值，并返回F3D
    F3D ret = {
        .x = sumX,
        .y = sumY,
        .z = sumZ,
    };
    return ret;
}

#endif
