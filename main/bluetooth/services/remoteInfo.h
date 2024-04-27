/*
 * File: \remoteInfo.h
 * Project: services
 * Created Date: 2024-03-11 22:55:56
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-27 13:19:04
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

/*
 * File: \remoteInfo.h
 * Project: services
 * Created Date: 2024-03-03 23:03:17
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-03 23:31:21
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

#include "../../globalStates/motionState.h"
#include "../../globalStates/BMSState.h"
#include "../../utils/F1D.h"

// ===========================远程信息服务 0x1022================================== //

/* 服务的uuid */
static const ble_uuid16_t gatt_remoteInfo_svc_uuid = BLE_UUID16_INIT(0x1022);

/* 服务内的characteristic */
// 1.基础运动参数
static uint32_t gatt_remoteInfo_chr_basic_motion_val[6];
static uint16_t gatt_remoteInfo_chr_basic_motion_val_handle;
static const ble_uuid16_t gatt_remoteInfo_chr_basic_motion_uuid = BLE_UUID16_INIT(0x1023);
// 2.电池电压
static uint16_t gatt_remoteInfo_chr_battery_voltage_val_handle;
static const ble_uuid16_t gatt_remoteInfo_chr_battery_voltage_uuid = BLE_UUID16_INIT(0x1024);

void calcBasicMotionVal()
{
    union F1D accx, accy, accz, gyrx, gyry, gyrz;
    accx.f = AccelData.x;
    accy.f = AccelData.y;
    accz.f = AccelData.z;
    gyrx.f = GyroData.x;
    gyry.f = GyroData.y;
    gyrz.f = GyroData.z;
    gatt_remoteInfo_chr_basic_motion_val[0] = accx.i;
    gatt_remoteInfo_chr_basic_motion_val[1] = accy.i;
    gatt_remoteInfo_chr_basic_motion_val[2] = accz.i;
    gatt_remoteInfo_chr_basic_motion_val[3] = gyrx.i;
    gatt_remoteInfo_chr_basic_motion_val[4] = gyry.i;
    gatt_remoteInfo_chr_basic_motion_val[5] = gyrz.i;
}

// 访问回调函数
static int gatt_remoteInfo_svc_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int gatt_remoteInfo_svc_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    int rc;

    switch (ctxt->op)
    {
    // 读操作 Append the value to ctxt->om if the operation is READ
    case BLE_GATT_ACCESS_OP_READ_CHR:
        if (attr_handle == gatt_remoteInfo_chr_basic_motion_val_handle)
        {
            calcBasicMotionVal();
            rc = os_mbuf_append(ctxt->om,
                                gatt_remoteInfo_chr_basic_motion_val,
                                sizeof(gatt_remoteInfo_chr_basic_motion_val));
            return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        } else if(attr_handle == gatt_remoteInfo_chr_battery_voltage_val_handle){
            rc = os_mbuf_append(ctxt->om,
                                &batteryVoltage,
                                sizeof(batteryVoltage));
            return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        }
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
