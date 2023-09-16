#include "main.h"

/**/
void fire_fighting()
{
		
    if(judge_result==fire_on_left){         //检测到左边有火
      FanMotor_Open();                      //开风扇
      turn_left45();                        //左转45
      delay_ms(1000);                       //对着目的点吹1000ms
      turn_left45_return();                 //回正
      FanMotor_Close();                     //关风扇
      FSMflag=TASKLOOP_FLAG;                 //灭完火之后让任务状态变为日常任务
    }

    else if(judge_result==fire_on_right){
      FanMotor_Open();
      turn_right45();
      delay_ms(1000);
      turn_right45_return();
      FanMotor_Close();
      FSMflag=TASKLOOP_FLAG;
    }
    
    else if(judge_result==no_fire){
      FanMotor_Close();
      return;
    }

  return ;
}
