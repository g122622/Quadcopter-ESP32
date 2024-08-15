/*
 * File: \remoteControll.h
 * Project: services
 * Created Date: 2024-03-03 23:03:17
 * Author: Guoyi
 * -----
 * Last Modified: 2024-05-21 18:34:27
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "host/ble_hs.h"
#include "host/ble_uuid.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "services/ans/ble_svc_ans.h"
#include "../utils/gatt_svr_write.h"
#include "utils/F1D.h"
#include "globalStates/controllerState.h"
#include "globalStates/PWMState.h"

#include "FlightController/PID/config/gyroPID.h"
#include "FlightController/PID/config/headingPID.h"
#include "FlightController/PID/config/throttlePID.h"

#include "FlightController/PID/clearI.h"

// ===========================遥控器服务 0xffe0================================== //

/* 服务的uuid */
static const ble_uuid16_t gatt_remoteControll_svc_uuid = BLE_UUID16_INIT(0xffe0);

/* 服务内的characteristic */
// 1.PID配置
// 4个PID配置，每个配置含3个float数据，再加一个积分上限和PID总输出上限共2个float
static uint32_t gatt_remoteControll_chr_PID_val[4 * 3 + 2];
static uint16_t gatt_remoteControll_chr_PID_val_handle;
static const ble_uuid16_t gatt_remoteControll_chr_PID_uuid = BLE_UUID16_INIT(0xffe1);
// 2.飞行状态
static uint16_t gatt_remoteControll_chr_flight_state_val_handle;
static const ble_uuid16_t gatt_remoteControll_chr_flight_state_uuid = BLE_UUID16_INIT(0xffe2);
// 3.PWM配置
static uint16_t gatt_remoteControll_chr_PWM_config_val_handle;
static const ble_uuid16_t gatt_remoteControll_chr_PWM_config_uuid = BLE_UUID16_INIT(0xffe3);

void convertPIDtoBle()
{
    union F1D gyroP, gyroI, gyroD, pitchP, pitchI, pitchD, rollP, rollI, rollD;

    gyroP.f = gyroPIDConfig.P_Weigh;
    gyroI.f = gyroPIDConfig.I_Weigh;
    gyroD.f = gyroPIDConfig.D_Weigh;
    pitchP.f = pitchPIDConfig.P_Weigh;
    pitchI.f = pitchPIDConfig.I_Weigh;
    pitchD.f = pitchPIDConfig.D_Weigh;
    rollP.f = rollPIDConfig.P_Weigh;
    rollI.f = rollPIDConfig.I_Weigh;
    rollD.f = rollPIDConfig.D_Weigh;

    gatt_remoteControll_chr_PID_val[0] = gyroP.i;
    gatt_remoteControll_chr_PID_val[1] = gyroI.i;
    gatt_remoteControll_chr_PID_val[2] = gyroD.i;
    gatt_remoteControll_chr_PID_val[3] = pitchP.i;
    gatt_remoteControll_chr_PID_val[4] = pitchI.i;
    gatt_remoteControll_chr_PID_val[5] = pitchD.i;
    gatt_remoteControll_chr_PID_val[6] = rollP.i;
    gatt_remoteControll_chr_PID_val[7] = rollI.i;
    gatt_remoteControll_chr_PID_val[8] = rollD.i;
}

void convertBletoPID()
{
    union F1D gyroP, gyroI, gyroD, pitchP, pitchI, pitchD, rollP, rollI, rollD, iLimit, totLimit;

    gyroP.i = gatt_remoteControll_chr_PID_val[0];
    gyroI.i = gatt_remoteControll_chr_PID_val[1];
    gyroD.i = gatt_remoteControll_chr_PID_val[2];
    pitchP.i = gatt_remoteControll_chr_PID_val[3];
    pitchI.i = gatt_remoteControll_chr_PID_val[4];
    pitchD.i = gatt_remoteControll_chr_PID_val[5];
    rollP.i = gatt_remoteControll_chr_PID_val[6];
    rollI.i = gatt_remoteControll_chr_PID_val[7];
    rollD.i = gatt_remoteControll_chr_PID_val[8];
    // TODO index等于9,10,11的情况
    iLimit.i = gatt_remoteControll_chr_PID_val[12];
    totLimit.i = gatt_remoteControll_chr_PID_val[13];

    gyroPIDConfig.P_Weigh = gyroP.f;
    gyroPIDConfig.I_Weigh = gyroI.f;
    gyroPIDConfig.D_Weigh = gyroD.f;
    pitchPIDConfig.P_Weigh = pitchP.f;
    pitchPIDConfig.I_Weigh = pitchI.f;
    pitchPIDConfig.D_Weigh = pitchD.f;
    rollPIDConfig.P_Weigh = rollP.f;
    rollPIDConfig.I_Weigh = rollI.f;
    rollPIDConfig.D_Weigh = rollD.f;
    PID_I_Limit = iLimit.f;
    PID_Total_Limit = totLimit.f;
}

// 读写回调函数
static int gatt_remoteControll_svc_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int gatt_remoteControll_svc_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    int rc;

    switch (ctxt->op)
    {
    // 读操作 Append the value to ctxt->om if the operation is READ
    case BLE_GATT_ACCESS_OP_READ_CHR:
        if (attr_handle == gatt_remoteControll_chr_PID_val_handle)
        {
            convertPIDtoBle();
            rc = os_mbuf_append(ctxt->om,
                                gatt_remoteControll_chr_PID_val,
                                sizeof(gatt_remoteControll_chr_PID_val));
            return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        }
        else if (attr_handle == gatt_remoteControll_chr_flight_state_val_handle)
        {
            rc = os_mbuf_append(ctxt->om,
                                &flightState,
                                sizeof(flightState));
            return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        }
        else if (attr_handle == gatt_remoteControll_chr_PWM_config_val_handle)
        {
            // TODO 扩充pwm1~4的读写
            uint32_t data[2]; // 2 * float
            union F1D basic, mult;
            basic.f = PWM_Basic;
            mult.f = PID_Mult;
            data[0] = basic.i;
            data[1] = mult.i;
            rc = os_mbuf_append(ctxt->om,
                                data,
                                sizeof(data));
            return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        }
        goto unknown;

    // 写操作 Write ctxt->om to the value if the operation is WRITE
    case BLE_GATT_ACCESS_OP_WRITE_CHR:
        if (attr_handle == gatt_remoteControll_chr_PID_val_handle)
        {
            rc = gatt_svr_write(ctxt->om,
                                sizeof(gatt_remoteControll_chr_PID_val),
                                sizeof(gatt_remoteControll_chr_PID_val),
                                gatt_remoteControll_chr_PID_val, NULL);
            // Send notification (or indication) to any connected devices that
            // have subscribed for notification (or indication) for specified characteristic.
            ble_gatts_chr_updated(attr_handle);
            convertBletoPID();
            // MODLOG_DFLT(INFO, "Notification/Indication scheduled for all subscribed peers.\n");
            return rc;
        }
        else if (attr_handle == gatt_remoteControll_chr_flight_state_val_handle)
        {
            rc = gatt_svr_write(ctxt->om,
                                sizeof(flightState),
                                sizeof(flightState),
                                &flightState, NULL);
            if (flightState == 1)
            {
                clearI();
            }
            // Send notification (or indication) to any connected devices that
            // have subscribed for notification (or indication) for specified characteristic.
            ble_gatts_chr_updated(attr_handle);    
            // MODLOG_DFLT(INFO, "Notification/Indication scheduled for all subscribed peers.\n");
            return rc;
        }
        else if (attr_handle == gatt_remoteControll_chr_PWM_config_val_handle)
        {
            uint32_t data[2 + 4]; // (2 + 4) * float
            rc = gatt_svr_write(ctxt->om,
                                sizeof(data),
                                sizeof(data),
                                data, NULL);
            union F1D basic, mult, p1m, p2m, p3m, p4m;
            basic.i = data[0];
            mult.i = data[1];
            p1m.i = data[2];
            p2m.i = data[3];
            p3m.i = data[4];
            p4m.i = data[5];
            
            PWM_Basic = basic.f;
            PID_Mult = mult.f;
            PWM1_Mult = p1m.f;
            PWM2_Mult = p2m.f;
            PWM3_Mult = p3m.f;
            PWM4_Mult = p4m.f;

            // Send notification (or indication) to any connected devices that
            // have subscribed for notification (or indication) for specified characteristic.
            ble_gatts_chr_updated(attr_handle);
            // MODLOG_DFLT(INFO, "Notification/Indication scheduled for all subscribed peers.\n");
            return rc;
        }
        goto unknown;

    case BLE_GATT_ACCESS_OP_READ_DSC:
        goto unknown;

    case BLE_GATT_ACCESS_OP_WRITE_DSC:
        goto unknown;

    default:
        goto unknown;
    }

unknown:
    /* Unknown characteristic/descriptor;
     * The NimBLE host should not have called this function;
     */
    assert(0);
    return BLE_ATT_ERR_UNLIKELY;
}
