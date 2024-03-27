/*
 * File: \motor.h
 * Project: motor
 * Created Date: 2024-03-27 16:27:48
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-27 16:49:02
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#include "../../PWMDriver/motorPWM.h"

#define DUTY_RANGE (4096)

float currentPercentage = 0;

void setMotorPWMPercentage(int motorNum, float percentage)
{
    if (percentage > 100)
        percentage = 100;
    if (percentage < 0)
        percentage = 0;
    setDuty(motorNum, (uint32_t)(0.01f * percentage * DUTY_RANGE));
    currentPercentage = percentage;
}

void changeMotorPWMPercentage(int motorNum, float delta)
{
    setMotorPWMPercentage(motorNum, currentPercentage + delta);
}
