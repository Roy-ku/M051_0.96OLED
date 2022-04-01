#ifndef __OLED_SSD1306_CONFIG_H__
#define __OLED_SSD1306_CONFIG_H__
#include "OLED_SSD1306_Driver.h"

/********************************************************************************
函式庫列表
********************************************************************************/

void SSD1306_SetPos(uint8_t x, uint8_t y);
void SSD1306_ShowStr(uint8_t x, uint8_t y, char ch[], uint8_t textsize);
void SSD1306_ShowNum(uint8_t x, uint8_t y, unsigned int num, uint8_t TextSize);
void SSD1306_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t *BMP);
void SSD1306_FILL(const uint8_t *BMP);
void SSD1306_Clear(void);
void SSD1306_Clear_Page(uint8_t page);
void SSD1306_ON(void);
void SSD1306_OFF(void);
void SSD1306_Run_Right(void);
void SSD1306_Vertical(void);
void SSD1306_ScrollStop(void);
#endif
