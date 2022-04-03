#include <stdio.h>
#include <string.h>
#include "M051Series.h"
#include "OLED_SSD1306_Config.h"
#include "User_Delay.h"
#include"Oled_bmp.h"
uint16_t count = 0;

///////////////////////////////////////////////////////

void Timer_Init()
{
	/* Enable TMR0 module clock */
	CLK_EnableModuleClock(TMR0_MODULE);
	/* Select TMR0 module clock source */
	CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT, CLK_CLKDIV_HCLK(1));

	/* Open Timer0 frequency to 0.5 Hz in periodic mode, and enable interrupt */
	TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1);
	TIMER_EnableInt(TIMER0);
	/* Enable Timer0 ~ Timer3 NVIC */
	NVIC_EnableIRQ(TMR0_IRQn);
}

void SYS_Init()
{
	/* Enable external XTAL 12MHz clock */
	CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);
	/* Waiting for clock ready */
	CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);
	/* Switch HCLK clock source to HXT*/
	CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HXT, CLK_CLKDIV_HCLK(1));
	/* Set core clock as PLL_CLOCK from PLL */
	CLK_SetCoreClock(50000000);
	SystemCoreClockUpdate();
}
///////////////////////////////////////////////////////
void Demo1()
{
	//extern uint8_t BMP1[];
	//SSD1306_DrawBMP(0, 0, 128, 8, BMP1);
	SSD1306_FILL(BMP1);
	Delay_ms(1000);

	char CPU[] = "CPU: ", CoreClock[10];
	sprintf(CoreClock, "%d", SystemCoreClock / 1000000);
	strcat(CPU, CoreClock);
	strcat(CPU, "MHZ");
	SSD1306_Clear();
	TIMER_Start(TIMER0);
	uint16_t count = strlen(CPU);

#if (TRANSFER_METHOD == HW_IIC)
	char I2C_C[] = "I2CClock:", I2C_Clock[10];
	sprintf(I2C_Clock, "%d", I2C_GetBusClockFreq(I2C0) / 1000);
	strcat(I2C_C, I2C_Clock);
	strcat(I2C_C, "KHZ");
	SSD1306_ShowStr(0, 5, "HW_IIC", SSD1306_TextSize_F6x8);
	SSD1306_ShowStr(0, 7, I2C_C, SSD1306_TextSize_F6x8);
#elif (TRANSFER_METHOD == SW_IIC)

	SSD1306_ShowStr(0, 5, "SW_IIC", SSD1306_TextSize_F6x8);
#endif

	SSD1306_ShowStr(0, 4, "Ver.002", SSD1306_TextSize_F6x8);
	SSD1306_ShowStr(64, 2, "0", SSD1306_TextSize_F8X16);

	while (1)
	{
		for (uint8_t i = 0; i < 127 - count * 6; i++)
		{
			TIMER_Stop(TIMER0);
			SSD1306_ShowStr(i, 0, CPU, SSD1306_TextSize_F6x8);
			TIMER_Start(TIMER0);
			Delay_ms(10);
		}

		for (uint8_t i = 127 - count * 6; i > 0; i--)
		{
			TIMER_Stop(TIMER0);
			SSD1306_ShowStr(i, 0, CPU, SSD1306_TextSize_F6x8);
			SSD1306_ShowStr(i + count * 6, 0, " ", SSD1306_TextSize_F6x8);
			TIMER_Start(TIMER0);
			Delay_ms(10);
		}
	}
}

int main(void)
{

	/* Unlock protected registers */
	SYS_UnlockReg();
	/* Init System, peripheral clock and multi-function I/O */
	SYS_Init();
	Timer_Init();
	Delay_Init();
	/* Lock protected registers */
	SYS_LockReg();

	SSD1306_Init();

	Demo1();
}

void TMR0_IRQHandler(void)
{
	if (TIMER_GetIntFlag(TIMER0) == 1)
	{
		char Count_num[10];
		/* Clear Timer0 time-out interrupt flag */
		TIMER_ClearIntFlag(TIMER0);
		if (count == 20000)
			count = 0;
		sprintf(Count_num, "%d", count);
		SSD1306_ShowStr(64, 2, Count_num, SSD1306_TextSize_F8X16);
		count++;
	}
}
