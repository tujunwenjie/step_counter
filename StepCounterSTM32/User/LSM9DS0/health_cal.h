#ifndef _HEALTH_CAL_H_
#define _HEALTH_CAL_H_

#include "math.h"
#include "main.h"
#include "stdio.h"
#include "ui.h"

#define FILTER_CNT			4      //均值滤波位数

#define MAX(a,b) ((a) > (b) ? (a) : (b)) 
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define SAMPLE_SIZE   50      //动态阈值更新数据间隔
#define ABS(a) (0 - (a)) > 0 ? (-(a)) : (a)

#define DYNAMIC_PRECISION     			0.1     	 /*动态精度*/

#define MOST_ACTIVE_NULL      			0      	 /*未找到最活跃轴*/
#define MOST_ACTIVE_X					1		 /*最活跃轴X*/	
#define MOST_ACTIVE_Y					2        /*最活跃轴Y*/	
#define MOST_ACTIVE_Z					3        /*最活跃轴Z*/	
 
#define ACTIVE_PRECISION      			0.3       /*活跃轴最小变化值*/


extern uint32_t tick1;
extern uint32_t tick2;      
extern uint8_t step_flag;   //步数是否规则标志位
extern uint8_t step_effect;   //连续有效步数个数，大于4则认为正常步行，计入步数
extern uint32_t step_cnt;
extern uint8_t res_old;   //上一次步数的活跃轴，用于判断步数是否规则
extern int time_r;
//均值滤波结构体
typedef struct {
  float x;
  float y;
  float z;
}axis_info_t;    //三轴加速度值

typedef struct filter_avg{
	axis_info_t info[FILTER_CNT];
	uint8_t count;
}filter_avg_t;

//动态阈值结构体
typedef struct {
	axis_info_t newmax;
	axis_info_t newmin;
	axis_info_t oldmax;
	axis_info_t oldmin;   //上一组样本计算出的最值
}peak_value_t;

/*一个线性移位寄存器，用于过滤高频噪声*/
typedef struct slid_reg{
	axis_info_t new_sample;
	axis_info_t old_sample;
}slid_reg_t;

extern filter_avg_t mean_filter;    //均值滤波寄存器
extern peak_value_t peak_val;   //动态阈值寄存器
extern slid_reg_t slid_reg;  //线性移位寄存器

void filter_update(filter_avg_t *filter,axis_info_t *cur_sample);
void Mean_filtering(filter_avg_t *filter, axis_info_t *sample);
 void peak_update(peak_value_t *peak, axis_info_t *cur_sample);
 char slid_update(slid_reg_t *slid, axis_info_t *cur_sample);
static char is_most_active(peak_value_t *peak);
 uint32_t detect_step(peak_value_t *peak, slid_reg_t *slid, axis_info_t *cur_sample);
#endif