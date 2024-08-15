/*
 * File: \controllerState.h
 * Project: globalStates
 * Created Date: 2024-04-05 21:25:11
 * Author: Guoyi
 * -----
 * Last Modified: 2024-05-09 17:39:34
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 * 
 * ------------------------------------
 */

#ifndef CONTROLLER_STATE_H
#define CONTROLLER_STATE_H

int __attribute__((weak)) flightState = 0;
float __attribute__((weak)) PID_I_Limit = 1;
float __attribute__((weak)) PID_Total_Limit = 50;

#endif
