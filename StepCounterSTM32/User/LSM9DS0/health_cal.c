#include "health_cal.h"


filter_avg_t mean_filter;    //均值滤波寄存器

//数字滤波器：首先需要一个数字滤波器来平滑信号。
//可以使用四个寄存器和一个求和单元，当然，也可以使用更多的
//寄存器来使加速度数据更平滑，但响应时间会更慢。

//更新均值滤波寄存器（先进先出）
void filter_update(filter_avg_t *filter,axis_info_t *cur_sample)
{
	for(uint8_t i=FILTER_CNT-1;i>0;i--)  //整体右移一格0->1->2->3
	{  
		filter->info[i]=filter->info[i-1];
	}
	filter->info[0].x=cur_sample->x;
	filter->info[0].y=cur_sample->y;
	filter->info[0].z=cur_sample->z;       //新数据存【0】
}

//均值滤波（4组数据取平均）
void Mean_filtering(filter_avg_t *filter, axis_info_t *sample)
{
	uint32_t i;
	float x_sum = 0, y_sum = 0, z_sum = 0;
	for (i = 0; i < FILTER_CNT; i++) 
	{
		x_sum += filter->info[i].x;
		y_sum += filter->info[i].y;
		z_sum += filter->info[i].z;
	}
	sample->x = x_sum / FILTER_CNT;
	sample->y = y_sum / FILTER_CNT;
	sample->z = z_sum / FILTER_CNT;
}

//动态阈值和动态精度：系统每 50 个样本连续更新 3 轴加速度的最大值和最小值。
//平均值 (最大值+最小值)/2 称为动态阈值水平。对于接下来的 50 个样本，此阈
//值水平用于决定是否已采取步骤。由于每 50 个样本更新一次，因此阈值是动态的。
//这种选择具有适应性并且足够快。除了动态阈值之外，动态精度还用于进一步过滤

//动态阈值
peak_value_t peak_val;   //动态阈值寄存器

static void peak_value_init(peak_value_t *peak)
{
	//在动态阈值结构体初始化时，一定要将max的值都赋值为最小值，
	//min赋值为最大值，这样才有利于动态更新。
	peak->newmax.x=-3000;
	peak->newmax.y=-3000;
	peak->newmax.z=-3000;
	peak->newmin.x=3000;
	peak->newmin.y=3000;
	peak->newmin.z=3000;
}
 void peak_update(peak_value_t *peak, axis_info_t *cur_sample)
{
  	static uint32_t sample_size = 0;
	  if(sample_size==0)
			peak_value_init(peak);
    sample_size ++;
	if (sample_size > SAMPLE_SIZE) 
	{
		/*采样达到50个，更新一次*/
		sample_size = 1;
		peak->oldmax = peak->newmax;
		peak->oldmin = peak->newmin;
      	//初始化
      	peak_value_init(peak);
	}
	peak->newmax.x = MAX(peak->newmax.x, cur_sample->x);
	peak->newmax.y = MAX(peak->newmax.y, cur_sample->y);
	peak->newmax.z = MAX(peak->newmax.z, cur_sample->z);
	
	peak->newmin.x = MIN(peak->newmin.x, cur_sample->x);
	peak->newmin.y = MIN(peak->newmin.y, cur_sample->y);
	peak->newmin.z = MIN(peak->newmin.z, cur_sample->z);
	
}

//动态精度：这其实是由一个线性移位寄存器加上一个我们预设定的动态变化值来消除高频噪声。
//其实它的操作很简单，移位寄存器中有两个样本，一个是new_sample,一个是old_sample.
//只要我们采集到一个样本，当前样本cur_sample.当cur_sample到来，old_sample无条件更新
//为new_sample的值，但是new_sample就不是无条件更新了，假设我们设定动态
//精度DYNAMIC_PRECISION=10，当|cur_sample.x-new_sample.x|，
//如果X轴变化量大于DYNAMIC_PRECISION，那么new_sample.x就更新为cur_sample.x，否则不变

slid_reg_t slid_reg;  //线性移位寄存器
 char slid_update(slid_reg_t *slid, axis_info_t *cur_sample)
{
  	char res = 0;
  	if (ABS((cur_sample->x - slid->new_sample.x)) > DYNAMIC_PRECISION) 
		{
			slid->old_sample.x = slid->new_sample.x;
			slid->new_sample.x = cur_sample->x;
			res = 1;
		} 
		else
	  {
		  slid->old_sample.x = slid->new_sample.x;
		}
	if (ABS((cur_sample->y - slid->new_sample.y)) > DYNAMIC_PRECISION)
	{
		slid->old_sample.y = slid->new_sample.y;
		slid->new_sample.y = cur_sample->y;
		res = 1;
	} 
	else 
	{
		slid->old_sample.y = slid->new_sample.y;
	}
	
	if (ABS((cur_sample->z - slid->new_sample.z)) > DYNAMIC_PRECISION) {
		slid->old_sample.z = slid->new_sample.z;
		slid->new_sample.z = cur_sample->z;
		res = 1;
	} else {
		slid->old_sample.z = slid->new_sample.z;
	}
	return res;
}


 
/*判断当前最活跃轴*/
static char is_most_active(peak_value_t *peak)
{
	char res = MOST_ACTIVE_NULL;
	float x_change = ABS((peak->newmax.x - peak->newmin.x));
	float y_change = ABS((peak->newmax.y - peak->newmin.y));
	float z_change = ABS((peak->newmax.z - peak->newmin.z));
	
	if (x_change > y_change && x_change > z_change && x_change >= ACTIVE_PRECISION) {
		res = MOST_ACTIVE_X;
	} else if (y_change > x_change && y_change > z_change && y_change >= ACTIVE_PRECISION) {
		res = MOST_ACTIVE_Y;
	} else if (z_change > x_change && z_change > y_change && z_change >= ACTIVE_PRECISION) {
		res = MOST_ACTIVE_Z;
	}
	return res;
}
 


//步伐判断：通过资料图，我们可以看到，判断为一步的条件是，先找到最活跃轴，
//然后最活跃轴的old_sample > 动态阈值，new_sample < 动态阈值。满足上述条件，
//认为走了一步。在我们运动过程，可以认为连续运动大于5步才认为是走步，这样可以过
//滤一些不必要的错误步数，同时资料所说的最快大约每秒5步，相当于200ms一步，我是
//利用每40ms取一次数据，200ms得到5组数据，滤波得到一个样本，这样可能更加准确一
//点。对于文档所说的时间窗口我就没有去做了，通过对比实验结果，我买的华为手环步数
//和我这个设备的步数基本一致，相差大约在200步以内
/*判断是否走步*/
uint32_t tick1=0;
uint32_t tick2=0;      
uint8_t step_flag=0;   //步数是否规则标志位
uint8_t step_effect=0;   //连续有效步数个数，大于4则认为正常步行，计入步数
uint32_t step_cnt = 0;
uint8_t res_old=MOST_ACTIVE_NULL;   //上一次步数的活跃轴，用于判断步数是否规则
int time_r=0;
uint8_t step_uneffect=0;
extern pedometer_params_t params;
uint32_t detect_step(peak_value_t *peak, slid_reg_t *slid, axis_info_t *cur_sample)
{
	char res = is_most_active(peak);
	tick1=HAL_GetTick();
	switch (res) {
		case MOST_ACTIVE_NULL: {
			//fix
			step_uneffect++;
			if(step_uneffect>=20)
			{
				params.static_state=0; //static
			}
			break;
		}
		case MOST_ACTIVE_X: {
			float threshold_x = (peak->oldmax.x + peak->oldmin.x) / 2;
			if (slid->old_sample.x > threshold_x && slid->new_sample.x < threshold_x) 
			{
				if(tick1-tick2>=200&&tick1-tick2<=2000)  //判断步频是否在0.2-2秒之间
				{
					//判断是否为连续规则有效步数
					if((res_old==res&&res_old!=MOST_ACTIVE_NULL)||res_old==MOST_ACTIVE_Y||res==MOST_ACTIVE_X)
					{
						step_uneffect=0;
						step_effect++;
						if(step_effect>=4)
						{						//连续四步有效步数后,判断发生规则步数，开始计步
							if(step_effect==4)
								step_cnt=step_cnt+4;
							step_flag=1;
						}
						else
							step_flag=0;
					}
					else
					{
						step_effect=0;
					}
					res_old=res;    
					
					
					if(step_flag)   //规则计步模式
					{
						step_cnt++;
						params.static_state=1;
						time_r=tick1-tick2;
					}
				}
				tick2=tick1;
			}
			break;
		}
		case MOST_ACTIVE_Y: {
			float threshold_y = (peak->oldmax.y + peak->oldmin.y) / 2;
			if (slid->old_sample.y > threshold_y && slid->new_sample.y < threshold_y) 
			{
				if(tick1-tick2>=200&&tick1-tick2<=2000)
				{
					if((res_old==res&&res_old!=MOST_ACTIVE_NULL)||res_old==MOST_ACTIVE_X||res==MOST_ACTIVE_Y)
					{
						step_uneffect=0;
						step_effect++;
						if(step_effect>=4)
						{						//连续四步有效步数后,判断发生规则步数，开始计步
							if(step_effect==4)
								step_cnt=step_cnt+4;
							step_flag=1;
						}
						else
							step_flag=0;
					}
					else
					{
						step_effect=0;
					}
					res_old=res;    
					
					
					if(step_flag)   //规则计步模式
					{
							step_cnt++;
							params.static_state=1;
							time_r=tick1-tick2;
					}
				}
				tick2=tick1;
			}
			break;
		}
		case MOST_ACTIVE_Z: {
			float threshold_z = (peak->oldmax.z + peak->oldmin.z) / 2;
			if (slid->old_sample.z > threshold_z && slid->new_sample.z < threshold_z) 
			{
				if(tick1-tick2>=200&&tick1-tick2<=2000)
				{
					if((res_old==res&&res_old!=MOST_ACTIVE_NULL))
					{
						step_uneffect=0;
						step_effect++;
						if(step_effect>=4)
						{						//连续四步有效步数后,判断发生规则步数，开始计步
							if(step_effect==4)
								step_cnt=step_cnt+4;
							step_flag=1;
						}
						else
							step_flag=0;
					}
					else
					{
						step_effect=0;
					}
					res_old=res;    
					
					
					if(step_flag)   //规则计步模式
					{
						step_cnt++;
						params.static_state=1;
						time_r=tick1-tick2;
					}
				}
				tick2=tick1;
			}
			break;
		}
		default: 
		{
			res_old=MOST_ACTIVE_NULL;
		}break;
	}
	//printf("Step_Cnt=%d\n",step_cnt);   
	return step_cnt;
}