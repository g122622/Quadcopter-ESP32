/*
 * File: \clearI.h
 * Project: PID
 * Created Date: 2024-05-02 23:59:14
 * Author: Guoyi
 * -----
 * Last Modified: 2024-05-03 00:07:11
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 * 
 * ------------------------------------
 */

#ifndef CLEAR_I_H
#define CLEAR_I_H

#include "./config/gyroPID.h"
#include "./config/headingPID.h"
#include "./config/throttlePID.h"

void clearI(){
    gyroPIDConfig.errIntegral = 0;
    pitchPIDConfig.errIntegral = 0;
    rollPIDConfig.errIntegral = 0;
    throttlePIDConfig.D_Weigh = 0;
}

#endif
