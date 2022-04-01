#ifndef __OLED_SSD1306_DRIVER_H__
#define __OLED_SSD1306_DRIVER_H__

#include <stdio.h>
#include <string.h>
#include "M051Series.h"
#include "SW_I2C.h"
#include "OLED_SSD1306_Config.h"

/*選擇通訊模式*/
#define HW_IIC (0) //目前無作用
#define SW_IIC (1)
#define HW_SPI (2) //目前無作用
#define SW_SPI (3) //目前無作用

#define TRANSFER_METHOD (SW_IIC)

#if (TRANSFER_METHOD == HW_IIC)
        #define SSD1306_ADDRESS (0x78) // 0x78和0x7A兩個地址 -- 默認0x78
        #define I2CX            I2C0
        #define IIC_SPEED           400000
#elif(TRANSFER_METHOD == SW_IIC)
        #define SSD1306_ADDRESS (0x78) // 0x78和0x7A兩個地址 -- 默認0x78
#elif(TRANSFER_METHOD == HW_SPI)

#elif(TRANSFER_METHOD == SW_SPI)

#endif
/********************************************************************************/

/*選擇螢幕類型*/
#define SSD1306_CUSTOMIZE (0) //自定義，目前無作用
#define SSD1306_128x32 (1)
#define SSD1306_128x64 (2)

#define SCREEN_TYPE (SSD1306_128x64)

#if (SCREEN_TYPE == SSD1306_128x32)
#define SCREEN_PAGE_NUM (4)
#define SCREEN_PAGEDATA_NUM (128)
#define SCREEN_COLUMN (128)
#define SCREEN_ROW (32)
#if 0
#define SCREEN_PHY_X ()
#define SCREEN_PHY_Y ()
#endif

#elif (SCREEN_TYPE == SSD1306_128x64)

#define SCREEN_PAGE_NUM (8)
#define SCREEN_PAGEDATA_NUM (128)
#define SCREEN_COLUMN (128)
#define SCREEN_ROW (64)
#if 0
#define SCREEN_PHY_X (21.744)
#define SCREEN_PHY_Y (10.864)
#endif

#elif (SCREEN_TYPE == CUSTOMIZE)
/*调整点阵分辨率128*64,宽度:128列，高度:64行*/
#define SCREEN_COLUMN (128)
#define SCREEN_ROW (64)
/*配置一帧数据能写多少页*/
#define SCREEN_PAGE_NUM (8)
/*配置一页能写多少数据*/
#define SCREEN_PAGEDATA_NUM (128)
#if 0
                            /*调整物理尺寸*/
#define SCREEN_PHY_X ()
#define SCREEN_PHY_Y ()
#endif
#endif
/********************************************************************************/

/*選擇字體大小*/
typedef enum
{
        SSD1306_TextSize_F6x8 = 1,
        SSD1306_TextSize_F8X16 = 2
} TEXTSIZE_TYPE;
/********************************************************************************/



/********************************************************************************
函式庫列表
********************************************************************************/
void SSD1306_Init(void);
void SSD1306_Continuous_Write(uint8_t *data);
uint8_t HW_I2C_Continuous_WriteByte(I2C_T *i2c, uint8_t *data);
uint8_t HW_I2C_WriteByte(I2C_T *i2c, uint8_t controlAddr, uint8_t data);
void SSD1306_Write(uint8_t controladdr, uint8_t data);
void SSD1306_Write_Cmd(uint8_t data);
void SSD1306_Write_Data(uint8_t data);
void IIC_Configuration(void);
void SW_IIC_Configuration(void);
void SPI_Configuration(void);
void SW_SPI_Configuration(void);
#endif
