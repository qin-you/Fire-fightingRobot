#include "main.h"

/**
  * @brief �ȴ�С���ҳ�����ֵ�ȶ���100~300mm
  */
void SenserOffsetInit(void)
{
  static u16 count=0;
  if(count%10==0)       //100ms
  {
    Get_Distance_Front();
  }
  if(count==200)        //2s
  {
    FSMflag=TASKLOOP_FLAG;
  }
  count++;
}

