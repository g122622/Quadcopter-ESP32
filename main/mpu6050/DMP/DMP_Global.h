/*
 * File: \DMP_Global.h
 * Project: DMP
 * Created Date: 2024-03-16 16:01:29
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-16 16:07:42
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 * 
 * ------------------------------------
 */

#define DMP_FEATURE_TAP (0x001)
#define DMP_FEATURE_ANDROID_ORIENT (0x002)
#define DMP_FEATURE_LP_QUAT (0x004)
#define DMP_FEATURE_PEDOMETER (0x008)
#define DMP_FEATURE_6X_LP_QUAT (0x010)
#define DMP_FEATURE_GYRO_CAL (0x020)
#define DMP_FEATURE_SEND_RAW_ACCEL (0x040)
#define DMP_FEATURE_SEND_RAW_GYRO (0x080)
#define DMP_FEATURE_SEND_CAL_GYRO (0x100)
#define DMP_FEATURE_SEND_ANY_GYRO (DMP_FEATURE_SEND_RAW_GYRO | \
                                   DMP_FEATURE_SEND_CAL_GYRO)
                                   
/* These defines are copied from dmpDefaultMPU6050.c in the general MPL
 * releases. These defines may change for each DMP image, so be sure to modify
 * these values when switching to a new image.
 */
#define CFG_LP_QUAT (2712)
#define END_ORIENT_TEMP (1866)
#define CFG_27 (2742)
#define CFG_20 (2224)
#define CFG_23 (2745)
#define CFG_FIFO_ON_EVENT (2690)
#define END_PREDICTION_UPDATE (1761)
#define CGNOTICE_INTR (2620)
#define X_GRT_Y_TMP (1358)
#define CFG_DR_INT (1029)
#define CFG_AUTH (1035)
#define UPDATE_PROP_ROT (1835)
#define END_COMPARE_Y_X_TMP2 (1455)
#define SKIP_X_GRT_Y_TMP (1359)
#define SKIP_END_COMPARE (1435)
#define FCFG_3 (1088)
#define FCFG_2 (1066)
#define FCFG_1 (1062)
#define END_COMPARE_Y_X_TMP3 (1434)
#define FCFG_7 (1073)
#define FCFG_6 (1106)
#define FLAT_STATE_END (1713)
#define SWING_END_4 (1616)
#define SWING_END_2 (1565)
#define SWING_END_3 (1587)
#define SWING_END_1 (1550)
#define CFG_8 (2718)
#define CFG_15 (2727)
#define CFG_16 (2746)
#define CFG_EXT_GYRO_BIAS (1189)
#define END_COMPARE_Y_X_TMP (1407)
#define DO_NOT_UPDATE_PROP_ROT (1839)
#define CFG_7 (1205)
#define FLAT_STATE_END_TEMP (1683)
#define END_COMPARE_Y_X (1484)
#define SKIP_SWING_END_1 (1551)
#define SKIP_SWING_END_3 (1588)
#define SKIP_SWING_END_2 (1566)
#define TILTG75_START (1672)
#define CFG_6 (2753)
#define TILTL75_END (1669)
#define END_ORIENT (1884)
#define CFG_FLICK_IN (2573)
#define TILTL75_START (1643)
#define CFG_MOTION_BIAS (1208)
#define X_GRT_Y (1408)
#define TEMPLABEL (2324)
#define CFG_ANDROID_ORIENT_INT (1853)
#define CFG_GYRO_RAW_DATA (2722)
#define X_GRT_Y_TMP2 (1379)

#define D_0_22 (22 + 512)
#define D_0_24 (24 + 512)

#define D_0_36 (36)
#define D_0_52 (52)
#define D_0_96 (96)
#define D_0_104 (104)
#define D_0_108 (108)
#define D_0_163 (163)
#define D_0_188 (188)
#define D_0_192 (192)
#define D_0_224 (224)
#define D_0_228 (228)
#define D_0_232 (232)
#define D_0_236 (236)

#define D_1_2 (256 + 2)
#define D_1_4 (256 + 4)
#define D_1_8 (256 + 8)
#define D_1_10 (256 + 10)
#define D_1_24 (256 + 24)
#define D_1_28 (256 + 28)
#define D_1_36 (256 + 36)
#define D_1_40 (256 + 40)
#define D_1_44 (256 + 44)
#define D_1_72 (256 + 72)
#define D_1_74 (256 + 74)
#define D_1_79 (256 + 79)
#define D_1_88 (256 + 88)
#define D_1_90 (256 + 90)
#define D_1_92 (256 + 92)
#define D_1_96 (256 + 96)
#define D_1_98 (256 + 98)
#define D_1_106 (256 + 106)
#define D_1_108 (256 + 108)
#define D_1_112 (256 + 112)
#define D_1_128 (256 + 144)
#define D_1_152 (256 + 12)
#define D_1_160 (256 + 160)
#define D_1_176 (256 + 176)
#define D_1_178 (256 + 178)
#define D_1_218 (256 + 218)
#define D_1_232 (256 + 232)
#define D_1_236 (256 + 236)
#define D_1_240 (256 + 240)
#define D_1_244 (256 + 244)
#define D_1_250 (256 + 250)
#define D_1_252 (256 + 252)
#define D_2_12 (512 + 12)
#define D_2_96 (512 + 96)
#define D_2_108 (512 + 108)
#define D_2_208 (512 + 208)
#define D_2_224 (512 + 224)
#define D_2_236 (512 + 236)
#define D_2_244 (512 + 244)
#define D_2_248 (512 + 248)
#define D_2_252 (512 + 252)

#define CPASS_BIAS_X (35 * 16 + 4)
#define CPASS_BIAS_Y (35 * 16 + 8)
#define CPASS_BIAS_Z (35 * 16 + 12)
#define CPASS_MTX_00 (36 * 16)
#define CPASS_MTX_01 (36 * 16 + 4)
#define CPASS_MTX_02 (36 * 16 + 8)
#define CPASS_MTX_10 (36 * 16 + 12)
#define CPASS_MTX_11 (37 * 16)
#define CPASS_MTX_12 (37 * 16 + 4)
#define CPASS_MTX_20 (37 * 16 + 8)
#define CPASS_MTX_21 (37 * 16 + 12)
#define CPASS_MTX_22 (43 * 16 + 12)
#define D_EXT_GYRO_BIAS_X (61 * 16)
#define D_EXT_GYRO_BIAS_Y (61 * 16) + 4
#define D_EXT_GYRO_BIAS_Z (61 * 16) + 8
#define D_ACT0 (40 * 16)
#define D_ACSX (40 * 16 + 4)
#define D_ACSY (40 * 16 + 8)
#define D_ACSZ (40 * 16 + 12)

#define FLICK_MSG (45 * 16 + 4)
#define FLICK_COUNTER (45 * 16 + 8)
#define FLICK_LOWER (45 * 16 + 12)
#define FLICK_UPPER (46 * 16 + 12)

#define D_AUTH_OUT (992)
#define D_AUTH_IN (996)
#define D_AUTH_A (1000)
#define D_AUTH_B (1004)

#define D_PEDSTD_BP_B (768 + 0x1C)
#define D_PEDSTD_HP_A (768 + 0x78)
#define D_PEDSTD_HP_B (768 + 0x7C)
#define D_PEDSTD_BP_A4 (768 + 0x40)
#define D_PEDSTD_BP_A3 (768 + 0x44)
#define D_PEDSTD_BP_A2 (768 + 0x48)
#define D_PEDSTD_BP_A1 (768 + 0x4C)
#define D_PEDSTD_INT_THRSH (768 + 0x68)
#define D_PEDSTD_CLIP (768 + 0x6C)
#define D_PEDSTD_SB (768 + 0x28)
#define D_PEDSTD_SB_TIME (768 + 0x2C)
#define D_PEDSTD_PEAKTHRSH (768 + 0x98)
#define D_PEDSTD_TIML (768 + 0x2A)
#define D_PEDSTD_TIMH (768 + 0x2E)
#define D_PEDSTD_PEAK (768 + 0X94)
#define D_PEDSTD_STEPCTR (768 + 0x60)
#define D_PEDSTD_TIMECTR (964)
#define D_PEDSTD_DECI (768 + 0xA0)

#define D_HOST_NO_MOT (976)
#define D_ACCEL_BIAS (660)

#define D_ORIENT_GAP (76)

#define D_TILT0_H (48)
#define D_TILT0_L (50)
#define D_TILT1_H (52)
#define D_TILT1_L (54)
#define D_TILT2_H (56)
#define D_TILT2_L (58)
#define D_TILT3_H (60)
#define D_TILT3_L (62)

// 下面的内容是为了方便移植而留下的，会和之前的mpu6050定义的一些宏有重叠
/* 寄存器映射表 */
struct gyro_reg_s
{
    unsigned char who_am_i;
    unsigned char rate_div;
    unsigned char lpf;
    unsigned char prod_id;
    unsigned char user_ctrl;
    unsigned char fifo_en;
    unsigned char gyro_cfg;
    unsigned char accel_cfg;

    // unsigned char accel_cfg2;

    // unsigned char lp_accel_odr;

    unsigned char motion_thr;
    unsigned char motion_dur;
    unsigned char fifo_count_h;
    unsigned char fifo_r_w;
    unsigned char raw_gyro;
    unsigned char raw_accel;
    unsigned char temp;
    unsigned char int_enable;
    unsigned char dmp_int_status;
    unsigned char int_status;

    // unsigned char accel_intel;

    unsigned char pwr_mgmt_1;
    unsigned char pwr_mgmt_2;
    unsigned char int_pin_cfg;
    unsigned char mem_r_w;
    unsigned char accel_offs;
    unsigned char i2c_mst;
    unsigned char bank_sel;
    unsigned char mem_start_addr;
    unsigned char prgm_start_h;
};
/* 硬件特性表 */
struct hw_s
{
    unsigned char addr;
    unsigned short max_fifo;
    unsigned char num_reg;
    unsigned short temp_sens;
    short temp_offset;
    unsigned short bank_size;
};

const struct gyro_reg_s reg_table = {
    0x75, // who_am_i
    0x19, // rate_div
    0x1A, // lpf
    0x0C, // prod_id
    0x6A, // user_ctrl
    0x23, // fifo_en
    0x1B, // gyro_cfg
    0x1C, // accel_cfg
    0x1F, // motion_thr
    0x20, // motion_dur
    0x72, // fifo_count_h
    0x74, // fifo_r_w
    0x43, // raw_gyro
    0x3B, // raw_accel
    0x41, // temp
    0x38, // int_enable
    0x39, //  dmp_int_status
    0x3A, //  int_status
    0x6B, // pwr_mgmt_1
    0x6C, // pwr_mgmt_2
    0x37, // int_pin_cfg
    0x6F, // mem_r_w
    0x06, // accel_offs
    0x24, // i2c_mst
    0x6D, // bank_sel
    0x6E, // mem_start_addr
    0x70  // prgm_start_h
};

const struct hw_s hw = {
    0x68, // addr
    1024, // max_fifo
    118,  // num_reg
    340,  // temp_sens
    -521, // temp_offset
    256   // bank_size
};