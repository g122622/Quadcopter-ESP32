/*
 * File: \PerformPID.h
 * Project: PID
 * Created Date: 2024-03-26 13:45:15
 * Author: Guoyi
 * -----
 * Last Modified: 2024-05-03 00:04:10
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef PERFORM_PID_H
#define PERFORM_PID_H

#include "./config/PIDConfig.h"
#include "utils/MathUtils.h"
#include "globalStates/controllerState.h"

float performPID(PIDConfig *configIn, float err, float dt)
{
    float P = configIn->P_Weigh * err;

    configIn->errIntegral += err * dt;
    float I = LIMIT(configIn->I_Weigh * configIn->errIntegral, PID_I_Limit);

    float D = configIn->D_Weigh * (err - configIn->lastErr) / dt;
    configIn->lastErr = err;

    return LIMIT(P + I + D, PID_Total_Limit);
}

#endif
