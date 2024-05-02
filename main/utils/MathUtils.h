/*
 * File: \math.h
 * Project: utils
 * Created Date: 2024-03-16 14:24:32
 * Author: Guoyi
 * -----
 * Last Modified: 2024-05-01 19:03:07
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 * 
 * ------------------------------------
 */

#ifndef MATHUTILS_H
#define MATHUTILS_H

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define LIMIT(X, Y) (MAX(MIN(X, Y), (-1) * Y)) // 将X的绝对值限制在Y以内

#endif
