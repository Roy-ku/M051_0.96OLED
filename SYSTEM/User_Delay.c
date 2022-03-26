#include "User_Delay.h"

static __IO uint32_t TimingDelay;
static __IO uint32_t Cycle_us;
/**************************************************************************************
 * 描 述 : Delay初始化設定
 * 入 參 : 無
 * 返回值 : 無
**************************************************************************************/
void Delay_Init()
{
/*選擇SysTick鐘源*/
#ifdef CLKSRC_is_Msk
	/****************************************
	 * SystemFrequency / 1000    1ms中斷一次
	 * SystemFrequency / 100000	 10us中斷一次
	 * SystemFrequency / 1000000 1us中斷一次
	 ****************************************/
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		/* Capture error */
		while (1)
			;
	}
	/* Disable SysTick counter */
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //關閉計數器
	SysTick->VAL = (0x00);					   //清空計數器
#else
	//使用外振(12MHz)
	SysTick->CTRL = (0 << SysTick_CTRL_CLKSOURCE_Pos);
	CLK->CLKSEL0 &= ~CLK_CLKSEL0_STCLK_S_Msk;
	CLK->CLKSEL0 |= CLK_CLKSEL0_STCLK_S_HXT_DIV2; //如果鐘源不是HCLK必須小於HCLK或HCLK/2
	Cycle_us = __HXT / 1000000 / 2;
#endif
}
/**************************************************************************************/
#ifdef CLKSRC_is_Msk

/**************************************************************************************
 * 描 述 : 延遲 ms(單位)
 * 入 參 : ms
 * 返回值 : 無
**************************************************************************************/
void Delay_ms(__IO uint32_t ms)
{
	TimingDelay = ms;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //開始計數器
	while (TimingDelay != 0)
		;
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //關閉計數器
}

/**************************************************************************************
 * 描 述 : 延遲 s(單位)
 * 入 參 : s
 * 返回值 : 無
**************************************************************************************/
void Delay_s(__IO uint32_t s)
{
	for (short int count = 0; count < s; count++)
	{
		Delay_ms(1000);
	}
}
/*中斷*/
void SysTick_Handler(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}
#else
/**************************************************************************************
 * 描 述 : 延遲 us(單位)
 * 入 參 : us
 * 返回值 :  無
**************************************************************************************/
void Delay_us(__IO uint32_t us)
{
	SysTick->LOAD = (us * Cycle_us) - 1;
	SysTick->VAL = (0x00);					  //清空計數器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //開始計數器
	/* Waiting for down-count to zero */
	while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
		;

	/* Disable SysTick counter */
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //關閉計數器
}
/**************************************************************************************
 * 描 述 : 延遲 ms(單位)
 * 入 參 : ms
 * 返回值 : 無
**************************************************************************************/
void Delay_ms(__IO uint32_t ms)
{
	Delay_us(1000 * ms);
}
/**************************************************************************************
 * 描 述 : 延遲 s(單位)
 * 入 參 : s
 * 返回值 : 無
**************************************************************************************/
void Delay_s(__IO uint32_t s)
{
	for (short int count = 0; count < s; count++)
	{
		Delay_us(1000000);
	}
}
#endif
