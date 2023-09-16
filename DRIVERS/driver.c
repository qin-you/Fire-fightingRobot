#include "main.h"

u8 FSMflag=SENSOROFFSET_FLAG;

void Driver_Init(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Usart1_Init(115200);
  LED_Init();
  Adc_Init();
  EXTIX_Init();
  PWM_Init();
  Wave_Init();
	FanMotor_Init();
  SysTick_Init();
	
}
