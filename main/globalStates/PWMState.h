/*
 * File: \PWMState.h
 * Project: globalStates
 * Created Date: 2024-03-31 16:10:19
 * Author: Guoyi
 * -----
 * Last Modified: 2024-05-02 16:53:30
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef PWM_STATE_H
#define PWM_STATE_H

float __attribute__((weak)) currentPWMPercentage[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float __attribute__((weak)) PID_Mult = 0.5;
float __attribute__((weak)) PWM_Basic = 30;

float __attribute__((weak)) PWM1_Mult = 1;
float __attribute__((weak)) PWM2_Mult = 1;
float __attribute__((weak)) PWM3_Mult = 1;
float __attribute__((weak)) PWM4_Mult = 1;

#endif
