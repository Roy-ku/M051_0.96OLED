#include <stdio.h>
#include <string.h>
#include "M051Series.h"
#include "OLED_SSD1306_Driver.h"
#include "User_Delay.h"

int count = 0;
char Count_num[10];
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

	TIMER_Start(TIMER0);
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

	extern unsigned char BMP1[];
	SSD1306_DrawBMP(0, 0, 128, 8, BMP1);
	// SSD1306_FILL(BMP1);
	Delay_ms(1000);

	char CPU[] = "CPU: ", CoreClock[10];
	// char I2C_C[] = "I2C_Clock: ", I2C_Clock[10];
	sprintf(CoreClock, "%d", SystemCoreClock / 1000000);

	strcat(CPU, CoreClock);
	strcat(CPU, "MHZ");
	// SSD1306_Clear_Page(2, SSD1306_TextSize_F8X16);
	SSD1306_Clear();
	uint16_t count = strlen(CPU);
	SSD1306_ShowStr(0, 4, "Ver.003", SSD1306_TextSize_F6x8);
	SSD1306_ShowStr(64, 2, "0", SSD1306_TextSize_F8X16);
	while (1)
	{

		// static int count = 0;
		// if (count == 20000)
		// 	count = 0;
		// if (count % 3)
		// {
		// 	if (count % 3 == 1)
		// 	{
		// 		SSD1306_Clear_Page(0);
		// 		SSD1306_Clear_Page(1);
		// 		SSD1306_ShowStr(0, 0, CPU, SSD1306_TextSize_F6x8);
		// 	}
		// }
		// else
		// {
		// 	// SSD1306_Clear_Page(0, SSD1306_TextSize_F8X16);
		// 	SSD1306_ShowStr(0, 0, "12345678123", SSD1306_TextSize_F8X16);
		// }
		// sprintf(Count_num, "%d", count);
		// SSD1306_ShowStr(64, 2, Count_num, SSD1306_TextSize_F8X16);
		// count++;
		// Delay_ms(1000);

		// for (uint8_t i = 0; i < 8; i++)
		// {
		//  	SSD1306_FILL(BMP1);
		//  	Delay_ms(500);
		// 	SSD1306_Clear_Page(i);
		// 	Delay_ms(500);
		// }
		for (int i = 0; i < 127 - count * 6; i++)
		{
			TIMER_Stop(TIMER0);
			SSD1306_ShowStr(i, 0, CPU, SSD1306_TextSize_F6x8);
			TIMER_Start(TIMER0);
			Delay_ms(10);
		}

		for (int i = 127 - count * 6; i > 0; i--)
		{
			TIMER_Stop(TIMER0);
			SSD1306_ShowStr(i, 0, CPU, SSD1306_TextSize_F6x8);
			SSD1306_ShowStr(i + count * 6, 0, " ", SSD1306_TextSize_F6x8);
			TIMER_Start(TIMER0);
			Delay_ms(10);
		}

	}
}

void TMR0_IRQHandler(void)
{
	if (TIMER_GetIntFlag(TIMER0) == 1)
	{
		/* Clear Timer0 time-out interrupt flag */
		TIMER_ClearIntFlag(TIMER0);
		if (count == 20000)
			count = 0;
		sprintf(Count_num, "%d", count);
		SSD1306_ShowStr(64, 2, Count_num, SSD1306_TextSize_F8X16);
		count++;
	}
}
