#ifndef __USER_DELAY_H__
#define __USER_DELAY_H__

#include "M051Series.h"
//#define CLKSRC_is_Msk //SysTick鐘源為HCLK鐘源

/****************************************************************
函式庫列表
*****************************************************************/
#ifdef CLKSRC_is_Msk
void Delay_Init(void);
void Delay_ms(__IO uint32_t ms);
void Delay_s(__IO uint32_t s);
#else
void Delay_Init(void);
void Delay_us(__IO uint32_t us);
void Delay_ms(__IO uint32_t ms);
void Delay_s(__IO uint32_t s);
#endif

#endif
