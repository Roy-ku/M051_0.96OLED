#ifndef __OLED_SSD1306_CONFIG_H__
#define __OLED_SSD1306_CONFIG_H__
#include "OLED_SSD1306_Driver.h"



/********************************************************************************
函式庫列表
********************************************************************************/
void SSD1306_Write(unsigned char addr, unsigned char data);
void SSD1306_Write_Cmd(unsigned char data);
void SSD1306_Write_Data(unsigned char data);
void SSD1306_SetPos(unsigned char x, unsigned char y);
void SSD1306_ShowStr(unsigned char x, unsigned char y, char ch[], unsigned char textsize);
void SSD1306_ShowNum(unsigned char x, unsigned char y, unsigned int num, unsigned char TextSize);
void SSD1306_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
void SSD1306_FILL(unsigned char BMP[]);
void SSD1306_Clear(void);
void SSD1306_Clear_Page(unsigned char page);
void SSD1306_ON(void);
void SSD1306_OFF(void);
void SSD1306_Run_Right(void);
void SSD1306_Vertical(void);
#endif
