/*
 * File: \motor.h
 * Project: motor
 * Created Date: 2024-03-27 16:27:48
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-04 17:30:02
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef FC_MOTOR_H
#define FC_MOTOR_H

#include "../../PWMDriver/motorPWM.h"
#include <globalStates/PWMState.h>

#define DUTY_RANGE (4096 / 2)

void setMotorPWMPercentage(int motorNum, float percentage)
{
    if (percentage > 100)
        percentage = 100;
    if (percentage < 0)
        percentage = 0;
    setDuty(motorNum, (uint32_t)(0.01f * percentage * DUTY_RANGE));
    currentPWMPercentage[motorNum] = percentage;
}

void changeMotorPWMPercentage(int motorNum, float delta)
{
    setMotorPWMPercentage(motorNum, currentPWMPercentage[motorNum] + delta);
}

#endif
