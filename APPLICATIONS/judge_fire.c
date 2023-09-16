#include "main.h"

u8 judge_result;        //全局变量用于存放判断火源的结果。
void judge_fire()       //判断是否有火源
{
  u16 tem;
  float temp1,temp3;
  tem=Get_Adc_Average(ADC_Channel_10,1);
  temp1=(float)tem*3.3/4096;
  printf("left: %f\r\n",temp1);
	
  tem=Get_Adc_Average(ADC_Channel_12,1);
  temp3=(float)tem*3.3/4096;
	printf("right %f\r\n",temp3);
	
  if(temp1<0.9){
    judge_result=fire_on_left;
    along_flag=0;
		FSMflag=FIRE_FLAG;
		
  }
  
  else if(temp3<0.9){
    judge_result=fire_on_right;
		along_flag=0;
		FSMflag=FIRE_FLAG;
		
  }
  else
    judge_result=no_fire;
		
}
