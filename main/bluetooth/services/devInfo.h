/*
 * File: \devInfo.h
 * Project: services
 * Created Date: 2024-03-03 12:20:04
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-03 23:08:04
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

// ===========================设备信息服务 0x180a================================== //

/* 服务的uuid */
static const ble_uuid16_t gatt_devinfo_svc_uuid = BLE_UUID16_INIT(0x180a);

/* 服务内的characteristic */
static uint8_t gatt_devinfo_chr_manuNameStr_val[] = "Quadcopter";
static uint16_t gatt_devinfo_chr_manuNameStr_val_handle;
static const ble_uuid16_t gatt_devinfo_chr_manuNameStr_uuid = BLE_UUID16_INIT(0x2a29);
static int gatt_devInfo_svc_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int gatt_devInfo_svc_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    if (ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR && attr_handle == gatt_devinfo_chr_manuNameStr_val_handle)
    {
        int rc = os_mbuf_append(ctxt->om, &gatt_devinfo_chr_manuNameStr_val, sizeof(gatt_devinfo_chr_manuNameStr_val));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }
    else
        return BLE_ATT_ERR_UNLIKELY;
}

static uint8_t gatt_devinfo_chr_SWRevStr_val[] = "+VERSION=V0.0.1";
static uint16_t gatt_devinfo_chr_SWRevStr_val_handle;
static const ble_uuid16_t gatt_devinfo_chr_SWRevStr_uuid = BLE_UUID16_INIT(0x2a28);
static int gatt_devInfo_SWRevStr_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int gatt_devInfo_SWRevStr_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    if (ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR && attr_handle == gatt_devinfo_chr_SWRevStr_val_handle)
    {
        int rc = os_mbuf_append(ctxt->om, &gatt_devinfo_chr_SWRevStr_val, sizeof(gatt_devinfo_chr_SWRevStr_val));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }
    else
        return BLE_ATT_ERR_UNLIKELY;
}

static uint8_t gatt_devinfo_chr_modelNumberStr_val[] = "Quadcopter-v0";
static uint16_t gatt_devinfo_chr_modelNumberStr_val_handle;
static const ble_uuid16_t gatt_devinfo_chr_modelNumberStr_uuid = BLE_UUID16_INIT(0x2a24);
static int gatt_devInfo_modelNumberStr_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int gatt_devInfo_modelNumberStr_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    if (ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR && attr_handle == gatt_devinfo_chr_modelNumberStr_val_handle)
    {
        int rc = os_mbuf_append(ctxt->om, &gatt_devinfo_chr_modelNumberStr_val, sizeof(gatt_devinfo_chr_modelNumberStr_val));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }
    else
        return BLE_ATT_ERR_UNLIKELY;
}