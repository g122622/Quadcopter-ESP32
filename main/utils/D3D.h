/*
 * File: \D3D.h
 * Project: utils
 * Created Date: 2024-03-11 13:56:26
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-11 22:52:25
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef D3D_H
#define D3D_H

struct D3D
{
    double x;
    double y;
    double z;
};

typedef struct D3D D3D; // 使用typedef关键字为结构体定义一个别名，规避conflicting types for 'D3D'; have 'struct <anonymous>'

#endif
