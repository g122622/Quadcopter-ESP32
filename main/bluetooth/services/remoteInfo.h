/*
 * File: \remoteInfo.h
 * Project: services
 * Created Date: 2024-03-11 22:55:56
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-11 23:45:51
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
#include "../../utils/D1D.h"

// ===========================远程信息服务 0x1022================================== //

/* 服务的uuid */
static const ble_uuid16_t gatt_remoteInfo_svc_uuid = BLE_UUID16_INIT(0x1022);

/* 服务内的characteristic */
// 基础运动参数
static uint64_t gatt_remoteInfo_chr_basic_motion_val[6];
static uint16_t gatt_remoteInfo_chr_basic_motion_val_handle;
static const ble_uuid16_t gatt_remoteInfo_chr_basic_motion_uuid = BLE_UUID16_INIT(0x1023);

void calcBasicMotionVal()
{
    union D1D accx, accy, accz, gyrx, gyry, gyrz;
    accx.d = AccelData.x;
    accy.d = AccelData.y;
    accz.d = AccelData.z;
    gyrx.d = GyroData.x;
    gyry.d = GyroData.y;
    gyrz.d = GyroData.z;
    gatt_remoteInfo_chr_basic_motion_val[0] = accx.i;
    gatt_remoteInfo_chr_basic_motion_val[1] = accy.i;
    gatt_remoteInfo_chr_basic_motion_val[2] = accz.i;
    gatt_remoteInfo_chr_basic_motion_val[3] = gyrx.i;
    gatt_remoteInfo_chr_basic_motion_val[4] = gyry.i;
    gatt_remoteInfo_chr_basic_motion_val[5] = gyrz.i;
}

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
                                &gatt_remoteInfo_chr_basic_motion_val,
                                sizeof(gatt_remoteInfo_chr_basic_motion_val));
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
