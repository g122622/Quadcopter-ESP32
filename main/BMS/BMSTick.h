/*
 * File: \BMSTick.h
 * Project: BMS
 * Created Date: 2024-04-28 22:52:22
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-28 23:10:33
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#include "../BMS/ADC/batteryVoltage.h"
#include "../globalStates/BMSState.h"
#include "../globalStates/controllerState.h"
#include "FlightController/motor/motor.h"
#include "LED/StatusLED.h"

#define BATTERY_VOLTAGE_LOW_THR (3200)
#define BATTERY_VOLTAGE_EXTREMELY_LOW_THR (3000)

void BMSTick()
{
    batteryVoltage = BMS_ADC_ReadBatteryVoltage();
    if (batteryVoltage != -1 && batteryVoltage < BATTERY_VOLTAGE_EXTREMELY_LOW_THR){
        flightState = 0;
        stopAllMotors(); // 多余的一步。在这里是为了万无一失，确保关停所有电机。
        flashStatusLED(10, 500);
    }
}
