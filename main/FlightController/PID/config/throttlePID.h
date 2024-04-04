/*
 * File: \throttlePID.h
 * Project: config
 * Created Date: 2024-03-26 20:22:59
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-31 19:11:46
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 * 
 * ------------------------------------
 */

#include "./PIDConfig.h"

PIDConfig throttlePIDConfig=
{
    .P_Weigh = 1,
    .I_Weigh = 0.1,
    .D_Weigh = 5,
    .lastErr = 0,
    .errIntegral = 0,
};