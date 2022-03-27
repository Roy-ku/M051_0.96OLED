#include "OLED_SSD1306_Driver.h"
///////////////////////////////////////////////////////
/**
 * @brief 初始化設定
 */
void SSD1306_Init(void)
{
/*GPIO初始化*/
#if (TRANSFER_METHOD == HW_IIC)
    I2C_Configuration();
#elif (TRANSFER_METHOD == HW_SPI)
    SPI_Configuration();
#elif (TRANSFER_METHOD == SW_SPI)
    SW_SPI_Configuration();
#elif (TRANSFER_METHOD == SW_IIC)
    SW_IIC_Configuration();
#endif

    CLK_SysTickDelay(100);   //必須延遲
    SSD1306_Write_Cmd(0xAE); //顯示關閉 0xAF是開啟 0xAE是關閉

#if (SCREEN_TYPE == SSD1306_128x32)

    SSD1306_Write_Cmd(0xA8); //設定分辨率
    SSD1306_Write_Cmd(0x1F); // 128*64:0x3F  128*32:0x1F
    SSD1306_Write_Cmd(0xDA); //設置COM硬體預設配置，適應分辨率
    SSD1306_Write_Cmd(0x02); // 0x12:0.96-128*64    0x02:0.96-128*32
#elif (SCREEN_TYPE == SSD1306_128x64)

    SSD1306_Write_Cmd(0xA8); //設定分辨率
    SSD1306_Write_Cmd(0x3F); // 128*64:0x3F  128*32:0x1F
    SSD1306_Write_Cmd(0xDA); //設置COM硬體預設配置，適應分辨率
    SSD1306_Write_Cmd(0x12); // 0x12:0.96-128*64    0x02:0.96-128*32
#endif

    SSD1306_Write_Cmd(0x20); //設置內存地址模式 有三種模式：水平，垂直，頁尋址（默認）
    SSD1306_Write_Cmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    SSD1306_Write_Cmd(0xB0); //為頁尋址模式設置頁面開始地址，0-7
    SSD1306_Write_Cmd(0xC8); //設置COM掃描方向 【0xc0上下反置COM0到COM N-1 左到右 】 【0xc8正常 COM N-1到COM0 右到左】
    SSD1306_Write_Cmd(0x00); //設置低列地址
    SSD1306_Write_Cmd(0x10); //設置高列地址
    SSD1306_Write_Cmd(0x40); //設置顯示開始行 0到63 第【5:0】位 01[xxxxx] 默認這五位是 000000b
    SSD1306_Write_Cmd(0x81); //對比度設置指令
    SSD1306_Write_Cmd(0xFF); //亮度 0x00~0xFF(亮度設置,越大越亮)
    SSD1306_Write_Cmd(0xA1); //設置segment重映射,對於IIC通訊的四腳OLED要設置成0xA1，如果設置成0xA1的話顯示會反置
    SSD1306_Write_Cmd(0xA6); //設置顯示方式,正常顯示:0xA6,反相顯示:0xA7
    SSD1306_Write_Cmd(0xA4); // 0xA4,輸出遵循RAM內容 0xA5,輸出忽略RAM內容
    SSD1306_Write_Cmd(0xD3); //設置顯示偏移
    SSD1306_Write_Cmd(0x00); //默認值00 沒有偏移
    SSD1306_Write_Cmd(0xD5); //設置顯示時鐘分頻/振盪器頻率
    SSD1306_Write_Cmd(0x80); //設置分率 0xF0
    SSD1306_Write_Cmd(0xD9); //設置預充電時期
    SSD1306_Write_Cmd(0x22); //默認0x22
    SSD1306_Write_Cmd(0xDB); // Set VCOMH Deselect Level 不是很懂，按照默認的設置就行了
    SSD1306_Write_Cmd(0x20); //默認是0x20 0.77xVcc
    SSD1306_Write_Cmd(0x8D); //充電泵設置
    SSD1306_Write_Cmd(0x14); // 0x14:允許在顯示開啟的時候使用 0x10:不允許在顯示開啟的時候使用
    SSD1306_Write_Cmd(0xAF); //顯示關閉 0xAF是開啟 0xAE是關閉
    SSD1306_Clear();
    // CLK_SysTickDelay(10);
}

/*初始化引腳*/
#if (TRANSFER_METHOD == HW_IIC)

void IIC_Configuration(void)
{
}
#elif (TRANSFER_METHOD == HW_SPI)

void SPI_Configuration(void)
{
}
#elif (TRANSFER_METHOD == SW_SPI)

void SW_SPI_Configuration(void)
{
}
#elif (TRANSFER_METHOD == SW_IIC)

/**
 * @brief 初始化SW_IIC
 */
void SW_IIC_Configuration(void)
{
    SW_IIC_GPIO_Config();
}
#endif
