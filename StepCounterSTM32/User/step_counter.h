#ifndef __STEP_COUNTER_H
#define __STEP_COUNTER_H

#include "main.h"
#include "lcd.h"
#include "health_cal.h"   // 你已有的滤波与加速度处理头文件

/*=============================*/
/*    对外可访问的变量声明     */
/*=============================*/

extern uint8_t step_running;       // 是否正在计步
extern uint32_t step_count;        // 当前步数
extern uint32_t step_max_count;    // 最大目标步数
extern uint32_t calorie;           // 卡路里消耗
extern uint32_t sport_time_sec;    // 运动时间（秒）
extern axis_info_t Accel_sample;   // 外部传入的滤波后加速度

/*=============================*/
/*        对外函数声明         */
/*=============================*/

void StepUI_Init(void);            // UI 初始化
void StepUI_Update(void);          // UI 刷新

void Step_Start(void);             // 开始计步
void Step_Stop(void);              // 停止计步
void Step_Process(void);           // 每次采样调用一次，用于判断步数

#endif
