/*
 * File: \gyroPID.h
 * Project: config
 * Created Date: 2024-03-30 13:50:14
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-12 11:13:51
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef GYRO_PID_H
#define GYRO_PID_H

#include "./PIDConfig.h"

PIDConfig __attribute__((weak)) gyroPIDConfig = {
    .P_Weigh = 1,
    .I_Weigh = 0,
    .D_Weigh = 5,
    .lastErr = 0,
    .errIntegral = 0,
};

#endif
