/*
 * File: \gatt_svr_write.h
 * Project: utils
 * Created Date: 2024-03-03 23:06:54
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-03 23:21:25
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

/* 公共的写入操作函数，所有service通用 */
static int gatt_svr_write(struct os_mbuf *om, uint16_t min_len, uint16_t max_len, void *dst, uint16_t *len)
{
    uint16_t om_len;
    int rc;

    // om是待拷贝的内存区域
    om_len = OS_MBUF_PKTLEN(om);
    if (om_len < min_len)
    {
        printf("gatt_svr_write::BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN, omLen = %d", om_len);
        return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
    }

    rc = ble_hs_mbuf_to_flat(om, dst, max_len, len);
    if (rc != 0)
    {
        printf("gatt_svr_write::buffer太小");
        return BLE_ATT_ERR_UNLIKELY;
    }

    return 0;
}