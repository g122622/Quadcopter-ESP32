/*
 * File: \PWMState.h
 * Project: globalStates
 * Created Date: 2024-03-31 16:10:19
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-28 15:22:00
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 * 
 * ------------------------------------
 */

#ifndef PWM_STATE_H
#define PWM_STATE_H

float __attribute__((weak)) currentPWMPercentage[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float __attribute__((weak)) PWM_Mult = 0.5;
float __attribute__((weak)) PWM_Basic = 70;

#endif
