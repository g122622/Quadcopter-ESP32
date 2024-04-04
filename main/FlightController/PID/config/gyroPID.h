/*
 * File: \gyroPID.h
 * Project: config
 * Created Date: 2024-03-30 13:50:14
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-31 19:22:06
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#include "./PIDConfig.h"

PIDConfig gyroPIDConfig = {
    .P_Weigh = 1,
    .I_Weigh = 0,
    .D_Weigh = 5,
    .lastErr = 0,
    .errIntegral = 0,
};
