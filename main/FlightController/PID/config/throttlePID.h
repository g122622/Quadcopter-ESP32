/*
 * File: \throttlePID.h
 * Project: config
 * Created Date: 2024-03-26 20:22:59
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-12 11:14:33
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef THROTTLE_PID_H
#define THROTTLE_PID_H

#include "./PIDConfig.h"

PIDConfig __attribute__((weak)) throttlePIDConfig = {
    .P_Weigh = 1,
    .I_Weigh = 0.1,
    .D_Weigh = 5,
    .lastErr = 0,
    .errIntegral = 0,
};

#endif
