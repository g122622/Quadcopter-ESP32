/*
 * File: \headingPID.h
 * Project: config
 * Created Date: 2024-03-26 20:22:24
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-04 19:26:16
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#include "./PIDConfig.h"

PIDConfig pitchPIDConfig = {
    .P_Weigh = 1,
    .I_Weigh = 0,
    .D_Weigh = 5,
    .lastErr = 0,
    .errIntegral = 0,
};

PIDConfig rollPIDConfig = {
    .P_Weigh = 2,
    .I_Weigh = 0,
    .D_Weigh = 10,
    .lastErr = 0,
    .errIntegral = 0,
};