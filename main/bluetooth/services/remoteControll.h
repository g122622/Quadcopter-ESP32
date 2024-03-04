/*
 * File: \remoteControll.h
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
#include "../utils/gatt_svr_write.h"

// ===========================遥控器服务 0xffe0================================== //

/* 服务的uuid */
static const ble_uuid16_t gatt_remoteControll_svc_uuid = BLE_UUID16_INIT(0xffe0);

/* 服务内的characteristic */
static uint8_t gatt_remoteControll_chr_val;
static uint16_t gatt_remoteControll_chr_val_handle;
static const ble_uuid16_t gatt_remoteControll_chr_uuid = BLE_UUID16_INIT(0xffe1);
static int gatt_remoteControll_svc_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int gatt_remoteControll_svc_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    int rc;

    switch (ctxt->op)
    {
    // 读操作 Append the value to ctxt->om if the operation is READ
    case BLE_GATT_ACCESS_OP_READ_CHR:
        if (attr_handle == gatt_remoteControll_chr_val_handle)
        {
            rc = os_mbuf_append(ctxt->om,
                                &gatt_remoteControll_chr_val,
                                sizeof(gatt_remoteControll_chr_val));
            return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        }
        goto unknown;

    // 写操作 Write ctxt->om to the value if the operation is WRITE
    case BLE_GATT_ACCESS_OP_WRITE_CHR:
        if (attr_handle == gatt_remoteControll_chr_val_handle)
        {
            rc = gatt_svr_write(ctxt->om,
                                sizeof(gatt_remoteControll_chr_val),
                                sizeof(gatt_remoteControll_chr_val),
                                &gatt_remoteControll_chr_val, NULL);
            ble_gatts_chr_updated(attr_handle);
            // MODLOG_DFLT(INFO, "Notification/Indication scheduled for all subscribed peers.\n");
            printf("%d \n", gatt_remoteControll_chr_val);
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
