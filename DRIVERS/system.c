#include "main.h"

/**
  * @brief 内联汇编代码，掩蔽所有中断
  * @param None
  * @retval None
  * @details 掩蔽期间中断被悬期，结束掩蔽后这些中断将被执行，若不想执行，请清除中断标志位
  */
void NVIC_SETPRIMASK(void)
{
  __asm__ volatile("CPSID i");
}

/**
  * @brief 内联汇编代码，结束掩蔽所有中断
  * @param None
  * @retval None
  * @details 掩蔽期间中断被悬期，结束掩蔽后这些中断将被执行，若不想执行，请清除中断标志位
  */
void NVIC_RESETPRIMASK(void)
{
  __asm__ volatile("CPSIE i");
}

/**
  *
  */
