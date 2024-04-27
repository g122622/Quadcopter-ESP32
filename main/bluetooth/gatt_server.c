/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "host/ble_hs.h"
#include "host/ble_uuid.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "bleprph.h"
#include "services/ans/ble_svc_ans.h"

// 服务
#include "./services/devInfo.h"
#include "./services/remoteControll.h"
#include "./services/remoteInfo.h"

// 全局变量
#include "../globalStates/motionState.h"

/*** Maximum number of characteristics with the notify flag ***/
#define MAX_NOTIFY 5

static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    {
        /*** 0x180a 设备信息服务 ***/
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &gatt_devinfo_svc_uuid.u,
        .characteristics = (struct ble_gatt_chr_def[]){

            {
                .uuid = &gatt_devinfo_chr_manuNameStr_uuid.u,
                .access_cb = gatt_devInfo_svc_access,
                .flags = BLE_GATT_CHR_F_READ,
                .val_handle = &gatt_devinfo_chr_manuNameStr_val_handle,
            },
            {
                .uuid = &gatt_devinfo_chr_modelNumberStr_uuid.u,
                .access_cb = gatt_devInfo_modelNumberStr_access,
                .flags = BLE_GATT_CHR_F_READ,
                .val_handle = &gatt_devinfo_chr_modelNumberStr_val_handle,
            },
            {
                .uuid = &gatt_devinfo_chr_SWRevStr_uuid.u,
                .access_cb = gatt_devInfo_SWRevStr_access,
                .flags = BLE_GATT_CHR_F_READ,
                .val_handle = &gatt_devinfo_chr_SWRevStr_val_handle,
            },
            {
                0, /* No more characteristics in this service. */
            }},
    },
    {
        /*** 0xffe0 遥控器通讯服务 ***/
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &gatt_remoteControll_svc_uuid.u,
        .characteristics = (struct ble_gatt_chr_def[]){

            {
                .uuid = &gatt_remoteControll_chr_PID_uuid.u,
                .access_cb = gatt_remoteControll_svc_access,
                .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY | BLE_GATT_CHR_F_WRITE_NO_RSP,
                .val_handle = &gatt_remoteControll_chr_PID_val_handle,
            },
            {
                0, /* No more characteristics in this service. */
            }},
    },
    {
        /*** 远程信息服务 0x1022 ***/
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &gatt_remoteInfo_svc_uuid.u,
        .characteristics = (struct ble_gatt_chr_def[]){

            {
                .uuid = &gatt_remoteInfo_chr_basic_motion_uuid.u,
                .access_cb = gatt_remoteInfo_svc_access,
                .flags = BLE_GATT_CHR_F_READ,
                .val_handle = &gatt_remoteInfo_chr_basic_motion_val_handle,
            },
            {
                .uuid = &gatt_remoteInfo_chr_battery_voltage_uuid.u,
                .access_cb = gatt_remoteInfo_svc_access,
                .flags = BLE_GATT_CHR_F_READ,
                .val_handle = &gatt_remoteInfo_chr_battery_voltage_val_handle,
            },
            {
                0, /* No more characteristics in this service. */
            }},
    },
    {
        0, /* No more services. */
    },
};

void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
    char buf[BLE_UUID_STR_LEN];

    switch (ctxt->op)
    {
    case BLE_GATT_REGISTER_OP_SVC:
        MODLOG_DFLT(DEBUG, "registered service %s with handle=%d\n",
                    ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf),
                    ctxt->svc.handle);
        break;

    case BLE_GATT_REGISTER_OP_CHR:
        MODLOG_DFLT(DEBUG, "registering characteristic %s with "
                           "def_handle=%d val_handle=%d\n",
                    ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf),
                    ctxt->chr.def_handle,
                    ctxt->chr.val_handle);
        break;

    case BLE_GATT_REGISTER_OP_DSC:
        MODLOG_DFLT(DEBUG, "registering descriptor %s with handle=%d\n",
                    ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf),
                    ctxt->dsc.handle);
        break;

    default:
        assert(0);
        break;
    }
}

// 初始化，并且注册services
int gatt_svr_init(void)
{
    int rc;

    ble_svc_gap_init();
    ble_svc_gatt_init();
    ble_svc_ans_init();

    rc = ble_gatts_count_cfg(gatt_svr_svcs);
    if (rc != 0)
    {
        return rc;
    }

    rc = ble_gatts_add_svcs(gatt_svr_svcs);
    if (rc != 0)
    {
        return rc;
    }

    return 0;
}
