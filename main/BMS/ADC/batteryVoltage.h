/*
 * File: \batteryVoltage.c
 * Project: ADC
 * Created Date: 2024-04-24 13:47:31
 * Author: Guoyi
 * -----
 * Last Modified: 2024-04-27 13:02:40
 * Modified By:
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

const static char *BMS_ADC_LOG_TAG = "BMS_ADC";

/*---------------------------------------------------------------
        ADC General Macros
---------------------------------------------------------------*/
// ADC1 Channels
#define BATTERY_ADC1_CHANNEL ADC_CHANNEL_7

#define BATTERY_ADC_ATTEN ADC_ATTEN_DB_12

static int adc_raw[2][10];
static int voltage[2][10];
static adc_oneshot_unit_handle_t adc1_handle;
static adc_cali_handle_t adc1_cali_handle = NULL;
static bool do_calibration1_chan0 = false;

static bool battery_adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle);
static void battery_adc_calibration_deinit(adc_cali_handle_t handle);

void BMS_ADC_Init(void)
{
    //-------------ADC1 Init---------------//
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc1_handle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .atten = BATTERY_ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, BATTERY_ADC1_CHANNEL, &config));

    //-------------ADC1 Calibration Init---------------//
    do_calibration1_chan0 = battery_adc_calibration_init(ADC_UNIT_1, BATTERY_ADC1_CHANNEL, BATTERY_ADC_ATTEN, &adc1_cali_handle);
}

// 如果读取数据时还未校准，那么返回-1
int BMS_ADC_ReadBatteryVoltage()
{
    if (do_calibration1_chan0)
    {
        adc_oneshot_read(adc1_handle, BATTERY_ADC1_CHANNEL, &adc_raw[0][0]);
        adc_cali_raw_to_voltage(adc1_cali_handle, adc_raw[0][0], &voltage[0][0]);
        return voltage[0][0] * 2; // 两个10k电阻分压
    }
    else
    {
        // 如果读取数据时还未校准，那么返回-1
        return -1;
    }
    return -1;
}

void BMS_ADC_Deinit()
{
    // Tear Down
    adc_oneshot_del_unit(adc1_handle);
    battery_adc_calibration_deinit(adc1_cali_handle);
}

/*---------------------------------------------------------------
        ADC Calibration
---------------------------------------------------------------*/
static bool
battery_adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

    if (!calibrated)
    {
        ESP_LOGI(BMS_ADC_LOG_TAG, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK)
        {
            calibrated = true;
        }
    }

    *out_handle = handle;
    if (ret == ESP_OK)
    {
        ESP_LOGI(BMS_ADC_LOG_TAG, "Calibration Success");
    }
    else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated)
    {
        ESP_LOGW(BMS_ADC_LOG_TAG, "eFuse not burnt, skip software calibration");
    }
    else
    {
        ESP_LOGE(BMS_ADC_LOG_TAG, "Invalid arg or no memory");
    }

    return calibrated;
}

static void battery_adc_calibration_deinit(adc_cali_handle_t handle)
{
    ESP_LOGI(BMS_ADC_LOG_TAG, "deregister %s calibration scheme", "Line Fitting");
    adc_cali_delete_scheme_line_fitting(handle);
}
