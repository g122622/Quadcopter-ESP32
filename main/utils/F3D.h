/*
 * File: \F3D.h
 * Project: utils
 * Created Date: 2024-03-11 13:56:26
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-12 13:35:41
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef F3D_H
#define F3D_H

struct F3D
{
    float x;
    float y;
    float z;
};

typedef struct F3D F3D; // 使用typedef关键字为结构体定义一个别名，规避conflicting types for 'F3D'; have 'struct <anonymous>'

#endif
