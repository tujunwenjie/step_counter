/*
 * pedometer_ui.h
 * 简易计步器界面头文件
 * 使用说明：在调用 PedometerUI_Init() 之前，可通过设置 lcd.h 中的
 *         BACK_COLOR (背景色) 和 POINT_COLOR (画笔/主色)
 * 来控制界面背景与主要数字颜色。
 */
#ifndef __PEDOMETER_UI_H
#define __PEDOMETER_UI_H

#include "lcd.h" // 使用 LCD 绘图接口与颜色变量
#include <stdint.h>

// 界面参数结构体（用于传入需要显示的值）
typedef struct {
    uint8_t start_flag;   // 0 停止, 1 运行
    uint16_t step_count;  // 当前步数
    uint16_t step_max;    // 目标步数
    uint32_t time_s;      // 运动时间（秒）
    float calories;       // 卡路里
    float speed;          // 速度（m/s）
    uint8_t static_state; // 0 移动, 1 静止
} pedometer_params_t;

// 初始化界面（清屏并绘制静态元素）
// 注意：调用此函数前可以修改 lcd.h 中的 BACK_COLOR(背景) 与 POINT_COLOR(主色)
void PedometerUI_Init(void);

// 更新动态字段（仅重绘发生变化的区域）
void PedometerUI_Update(const pedometer_params_t *p);

#endif // __PEDOMETER_UI_H
