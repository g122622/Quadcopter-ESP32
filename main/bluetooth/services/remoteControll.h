/*
 * File: \remoteControll.h
 * Project: services
 * Created Date: 2024-03-03 23:03:17
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-12 11:04:45
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
#include "../../utils/F1D.h"

#include "FlightController/PID/config/gyroPID.h"
#include "FlightController/PID/config/headingPID.h"
#include "FlightController/PID/config/throttlePID.h"

// ===========================遥控器服务 0xffe0================================== //

/* 服务的uuid */
static const ble_uuid16_t gatt_remoteControll_svc_uuid = BLE_UUID16_INIT(0xffe0);

/* 服务内的characteristic */
// 1.PID配置
static uint32_t gatt_remoteControll_chr_PID_val[4 * 3]; // 4个PID配置，每个配置含3个float数据
static uint16_t gatt_remoteControll_chr_PID_val_handle;
static const ble_uuid16_t gatt_remoteControll_chr_PID_uuid = BLE_UUID16_INIT(0xffe1);

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
    union F1D gyroP, gyroI, gyroD, pitchP, pitchI, pitchD, rollP, rollI, rollD;

    gyroP.i = gatt_remoteControll_chr_PID_val[0];
    gyroI.i = gatt_remoteControll_chr_PID_val[1];
    gyroD.i = gatt_remoteControll_chr_PID_val[2];
    pitchP.i = gatt_remoteControll_chr_PID_val[3];
    pitchI.i = gatt_remoteControll_chr_PID_val[4];
    pitchD.i = gatt_remoteControll_chr_PID_val[5];
    rollP.i = gatt_remoteControll_chr_PID_val[6];
    rollI.i = gatt_remoteControll_chr_PID_val[7];
    rollD.i = gatt_remoteControll_chr_PID_val[8];

    gyroPIDConfig.P_Weigh = gyroP.f;
    gyroPIDConfig.I_Weigh = gyroI.f;
    gyroPIDConfig.D_Weigh = gyroD.f;
    pitchPIDConfig.P_Weigh = pitchP.f;
    pitchPIDConfig.I_Weigh = pitchI.f;
    pitchPIDConfig.D_Weigh = pitchD.f;
    rollPIDConfig.P_Weigh = rollP.f;
    rollPIDConfig.I_Weigh = rollI.f;
    rollPIDConfig.D_Weigh = rollD.f;
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
