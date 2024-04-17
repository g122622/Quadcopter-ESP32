/*
 * File: \headingPID.h
 * Project: config
 * Created Date: 2024-03-26 20:22:24
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-12 11:14:18
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef HEADING_PID_H
#define HEADING_PID_H

#include "./PIDConfig.h"

PIDConfig __attribute__((weak)) pitchPIDConfig = {
    .P_Weigh = 1,
    .I_Weigh = 0,
    .D_Weigh = 2.5,
    .lastErr = 0,
    .errIntegral = 0,
};

PIDConfig __attribute__((weak)) rollPIDConfig = {
    .P_Weigh = 1,
    .I_Weigh = 0,
    .D_Weigh = 2.5,
    .lastErr = 0,
    .errIntegral = 0,
};

#endif
