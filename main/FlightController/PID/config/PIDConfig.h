/*
 * File: \PIDConfig.h
 * Project: Config
 * Created Date: 2024-03-26 13:46:46
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-26 13:51:33
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef PID_CONFIG_H
#define PID_CONFIG_H

struct PIDConfig
{
    float lastErr;
    float errIntegral;
    float P_Weigh;
    float I_Weigh;
    float D_Weigh;
};

#endif
