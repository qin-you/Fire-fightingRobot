#include "main.h"

static u8  fac_us=0;              //us延时倍乘数

void SysTick_Init()
{
  u32 reload=0;
  fac_us=SystemCoreClock/8000000;                          //每个us需要的systick时钟数
  reload=SystemCoreClock/8000*10;                           //reload值为10ms需要的systick时钟数
  SysTick->LOAD  = (reload & SysTick_LOAD_RELOAD_Msk) - 1; //每10ms中断一次
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
  SysTick->VAL   = 0;
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);    //选择外部时钟  HCLK/8
  SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;                 //开始SYSTICK中断
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;                  //开始SYSTICK计数
}

void delay_us(u32 nus)
{
  NVIC_SETPRIMASK();
  u32 tick=nus*fac_us;
  u32 time_old,time_now,time_cnt=0;
  u32 reload=SysTick->LOAD;

  time_old=SysTick->VAL;
  while(1)
  {
    time_now=SysTick->VAL;
    if(time_now<=time_old)
      time_cnt+=time_old-time_now;
    else
      time_cnt+=reload-time_now+time_old;

    time_old=time_now;
    if(time_cnt>=tick)
      break;
  }
	NVIC_RESETPRIMASK();
}

void delay_ms(u16 nms)
{
  delay_us((u32)(1000*nms));
} 


//注释掉了位于stm32f10x_it.c 136行的SysTick_Handler(),避免重定义。
void SysTick_Handler(void)
{

  switch(FSMflag)
  {
    case SENSOROFFSET_FLAG: SenserOffsetInit();break;
    case TASKLOOP_FLAG:     taskloop();break;
    case FIRE_FLAG:         fire_fighting();break;
  }
}
