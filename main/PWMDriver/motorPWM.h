/*
 * File: \motorPWM.h
 * Project: PWMDriver
 * Created Date: 2024-03-26 21:11:19
 * Author: Guoyi
 * -----
 * Last Modified: 2024-03-29 12:46:40
 * Modified By: Guoyi
 * -----
 * Copyright (c) 2024 Guoyi Inc.
 *
 * ------------------------------------
 */

#ifndef PWM_MOTOR_H
#define PWM_MOTOR_H

#include "driver/ledc.h"

// 时钟源和channel配置
#define LEDC_MODE LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_GPIO (32)
#define LEDC_HS_CH0_CHANNEL LEDC_CHANNEL_0
#define LEDC_HS_CH1_GPIO (33)
#define LEDC_HS_CH1_CHANNEL LEDC_CHANNEL_1
#define LEDC_HS_CH2_GPIO (25)
#define LEDC_HS_CH2_CHANNEL LEDC_CHANNEL_2
#define LEDC_HS_CH3_GPIO (26)
#define LEDC_HS_CH3_CHANNEL LEDC_CHANNEL_3

#define LEDC_CH_NUM (4)   // 总的PWM通道数
#define LEDC_FREQ (10000) // 频率

ledc_channel_config_t ledc_channels[LEDC_CH_NUM];

void MotorPWMDriver_Init(void)
{
    int ch;

    // 配置timers
    ledc_timer_config_t ledc_timer0 = {
        .duty_resolution = LEDC_TIMER_12_BIT, // 占空比分辨率
        .freq_hz = LEDC_FREQ,                 // PWM信号频率
        .speed_mode = LEDC_MODE,              // timer mode
        .timer_num = LEDC_TIMER_0,            // timer index
        .clk_cfg = LEDC_AUTO_CLK,             // 自动选择 source clock
    };
    ledc_timer_config(&ledc_timer0);

    ledc_timer_config_t ledc_timer1 = {
        .duty_resolution = LEDC_TIMER_12_BIT, // 占空比分辨率
        .freq_hz = LEDC_FREQ,                 // PWM信号频率
        .speed_mode = LEDC_MODE,              // timer mode
        .timer_num = LEDC_TIMER_1,            // timer index
        .clk_cfg = LEDC_AUTO_CLK,             // 自动选择 source clock
    };
    ledc_timer_config(&ledc_timer1);

    ledc_timer_config_t ledc_timer2 = {
        .duty_resolution = LEDC_TIMER_12_BIT, // 占空比分辨率
        .freq_hz = LEDC_FREQ,                 // PWM信号频率
        .speed_mode = LEDC_MODE,              // timer mode
        .timer_num = LEDC_TIMER_2,            // timer index
        .clk_cfg = LEDC_AUTO_CLK,             // 自动选择 source clock
    };
    ledc_timer_config(&ledc_timer2);

    ledc_timer_config_t ledc_timer3 = {
        .duty_resolution = LEDC_TIMER_12_BIT, // 占空比分辨率
        .freq_hz = LEDC_FREQ,                 // PWM信号频率
        .speed_mode = LEDC_MODE,              // timer mode
        .timer_num = LEDC_TIMER_3,            // timer index
        .clk_cfg = LEDC_AUTO_CLK,             // 自动选择 source clock
    };
    ledc_timer_config(&ledc_timer3);

    /*
    * 为LED控制器的每个通道准备单独的配置，通过选择：
    - 控制器的通道号
    - 输出占空比，初始设置为0
    - LED连接的GPIO编号
    - 速度模式，高或低
    - 选择的通道计时器
    * 注意：如果不同通道使用一个计时器，那么这些通道的频率和位数是相同的
     */
    ledc_channel_config_t ledc_channels[LEDC_CH_NUM] = {
        {.channel = LEDC_HS_CH0_CHANNEL,
         .duty = 0,
         .gpio_num = LEDC_HS_CH0_GPIO,
         .speed_mode = LEDC_MODE,
         .hpoint = 0,
         .timer_sel = LEDC_TIMER_0,
         .flags.output_invert = 0},
        {.channel = LEDC_HS_CH1_CHANNEL,
         .duty = 0,
         .gpio_num = LEDC_HS_CH1_GPIO,
         .speed_mode = LEDC_MODE,
         .hpoint = 0,
         .timer_sel = LEDC_TIMER_1,
         .flags.output_invert = 0},
        {.channel = LEDC_HS_CH2_CHANNEL,
         .duty = 0,
         .gpio_num = LEDC_HS_CH2_GPIO,
         .speed_mode = LEDC_MODE,
         .hpoint = 0,
         .timer_sel = LEDC_TIMER_2,
         .flags.output_invert = 0},
        {.channel = LEDC_HS_CH3_CHANNEL,
         .duty = 0,
         .gpio_num = LEDC_HS_CH3_GPIO,
         .speed_mode = LEDC_MODE,
         .hpoint = 0,
         .timer_sel = LEDC_TIMER_3,
         .flags.output_invert = 0},
    };

    // 对每一个channel应用设置
    for (ch = 0; ch < LEDC_CH_NUM; ch++)
    {
        ledc_channel_config(&ledc_channels[ch]);
    }
}

/**
 * 设置电机PWM占空比
 * motorNum的取值为到3
 */
void setDuty(int motorNum, uint32_t duty)
{
    ledc_set_duty(ledc_channels[motorNum].speed_mode, ledc_channels[motorNum].channel, duty);
    ledc_update_duty(ledc_channels[motorNum].speed_mode, ledc_channels[motorNum].channel);
}

#endif
