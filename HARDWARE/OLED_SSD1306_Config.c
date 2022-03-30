#include "OLED_SSD1306_Config.h"
#include "Font.h"

// unsigned char ScreenBuffer[SCREEN_PAGE_NUM][SCREEN_COLUMN] = {0};

#if (TRANSFER_METHOD == HW_IIC)
uint8_t I2C_WriteByte(I2C_T *i2c, uint8_t controlAddr, uint8_t data)
{
    uint8_t u8Xfering = 1, u8Err = 0, u8Ctrl = 0, u8length = 0;

    I2C_START(i2c);
    while (u8Xfering && (u8Err == 0))
    {
        I2C_WAIT_READY(i2c);
        switch (I2C_GET_STATUS(i2c))
        {
        /*發送START信號成功*/
        case 0x08:
            I2C_SET_DATA(i2c, SSD1306_ADDRESS); /* Write SLA+W to Register I2CDAT */
            u8Ctrl = I2C_I2CON_SI;              /* Clear SI */
            break;
        /*發送地址+W成功並收到ACK*/
        case 0x18:                          /* Slave Address ACK */
            I2C_SET_DATA(i2c, controlAddr); /* Write data or cmd */
            u8Ctrl = I2C_I2CON_SI;          /* Clear SI */
            break;
        /*發送數據成功並收到ACK*/
        case 0x28:
            if (u8length < 1)
            {
                I2C_SET_DATA(i2c, data); /* Write data to I2CDAT */
                u8Ctrl = I2C_I2CON_SI;   /* Clear SI */
                u8length++;
            }
            else
            {
                u8Ctrl = I2C_I2CON_STO_SI; /* Clear SI and send STOP */
                u8Xfering = 0;
            }
            break;
        /*發送地址+W成功並收到NACK*/
        case 0x20:                     /* Slave Address NACK */
                                       /*發送數據成功並收到NACK*/
        case 0x30:                     /* Master transmit data NACK */
                                       /*Master發生仲裁失敗*/
        case 0x38:                     /* Arbitration Lost */
        default:                       /* Unknow status */
            u8Ctrl = I2C_I2CON_STO_SI; /* Clear SI and send STOP */
            u8Err = 1;
            break;
        }
        I2C_SET_CONTROL_REG(i2c, u8Ctrl); /* Write controlbit to I2C_CTL register */
    }
    return (u8Err | u8Xfering); /* return (Success)/(Fail) status */
}

void SSD1306_Write_Cmd(unsigned char data)
{
    //SSD1306_Write(0x00,data);
    I2C_WriteByte(I2CX, 0x00, data);
}

void SSD1306_Write_Data(unsigned char data)
{
    //SSD1306_Write(0x40,data);
    I2C_WriteByte(I2CX, 0x40, data);
}

#elif (TRANSFER_METHOD == SW_IIC)
void SSD1306_Write(unsigned char controladdr, unsigned char data)
{
    SW_IIC_Start();
    SW_IIC_Write_Byte(SSD1306_ADDRESS);
    SW_IIC_WaitAck();
    SW_IIC_Write_Byte(controladdr);
    SW_IIC_WaitAck();
    SW_IIC_Write_Byte(data);
    SW_IIC_WaitAck();
    SW_IIC_Stop();
}

void SSD1306_Write_Cmd(unsigned char data)
{
    SSD1306_Write(0x00, data);
}

void SSD1306_Write_Data(unsigned char data)
{
    SSD1306_Write(0x40, data);
}

#endif

/**
 * @brief 設置起始座標
 * @param x x座標
 * @param y y座標
 */
void SSD1306_SetPos(unsigned char x, unsigned char y)
{
    SSD1306_Write_Cmd(0xB0 + y);
    SSD1306_Write_Cmd(((x & 0xF0) >> 4) | 0x10);
    SSD1306_Write_Cmd((x & 0x0F) | 0x00);
}

/**
 * @brief 顯示字串
 * @param x x座標
 * @param y y座標
 * @param ch 需顯示的字串
 * @param textsize 字體大小
 */
void SSD1306_ShowStr(unsigned char x, unsigned char y, char ch[], unsigned char textsize)
{
    unsigned char c = 0, j = 0;

    switch (textsize)
    {
    case SSD1306_TextSize_F6x8:
    {
        while (ch[j] != '\0')
        {
            c = ch[j] - 32;
            if (x >= 127)
            {
                x = 0;
                // y++;
            }
            SSD1306_SetPos(x, y);
            for (unsigned char i = 0; i < 6; i++)
            {
                SSD1306_Write_Data(F6x8[c * 6 + i]);
            }

            // SW_IIC_Start();
            // SW_IIC_Write_Byte(SSD1306_ADDRESS);
            // SW_IIC_WaitAck();
            // SW_IIC_Write_Byte(0x40);
            // SW_IIC_WaitAck();
            // for (unsigned char i = 0; i < 6; i++)
            // {
            //     SW_IIC_Write_Byte(F6x8[c * 6 + i]);
            //     SW_IIC_WaitAck();
            // }
            // SW_IIC_Stop();
            x += 6;
            j++;
        }
    }
    break;

    case SSD1306_TextSize_F8X16:
    {
        while (ch[j] != '\0')
        {
            c = ch[j] - 32;
            if (x >= 127)
            {
                x = 0;
                // y++;
            }
            SSD1306_SetPos(x, y);
            for (unsigned char i = 0; i < 8; i++)
            {
                SSD1306_Write_Data(F8X16[c * 16 + i]);
            }
            SSD1306_SetPos(x, y + 1);
            for (unsigned char i = 0; i < 8; i++)
            {
                SSD1306_Write_Data(F8X16[c * 16 + i + 8]);
            }
            x += 8;
            j++;
        }
    }
    break;
    }
}

/**
 * @brief 顯示數字
 * @param x x座標
 * @param y y座標
 * @param num 需顯示的數字
 * @param TextSize 字體大小
 * @note  從右至左
 */
void SSD1306_ShowNum(unsigned char x, unsigned char y, unsigned int num, unsigned char TextSize)
{
    unsigned char c = 0, i = 0;
    short count = 0;
    char str[5] = "", str_n[5];
    snprintf(str_n, sizeof(str_n), "%d", num);

    /*無效0*/
    for (int i = strlen(str_n); i < 4; i++)
    {
        strcat(str, " ");
    }
    strcat(str, str_n);
    count = strlen(str);

    switch (TextSize)
    {
    case 1:
    {
        while (count != 0)
        {
            c = str[count - 1] - 32;
            if (x > 127)
            {
                x = 0;
                // y++; //當x溢位時是否換行
            }
            SSD1306_SetPos(x, y);

            for (i = 0; i < 6; i++)
            {
                SSD1306_Write_Data(F6x8[c * 6 + i]);
            }
            x -= 6;
            count--;
        }
    }
    break;

    case 2:
    {
        while (count != 0)
        {
            c = str[count - 1] - 32;
            if (x > 127)
            {
                x = 0;
                // y++; //當x溢位時是否換行
            }
            SSD1306_SetPos(x, y);
            for (i = 0; i < 8; i++)
            {
                SSD1306_Write_Data(F8X16[c * 16 + i]);
            }
            SSD1306_SetPos(x, y + 1);
            for (i = 0; i < 8; i++)
            {
                SSD1306_Write_Data(F8X16[c * 16 + i + 8]);
            }
            x -= 8;
            count--;
        }
    }
    break;
    }
}

/**
 * @brief 顯示BMP位圖
 * @param x0 起點座標x0
 * @param y0 起點座標y0
 * @param x1 結束座標x1
 * @param y1 結束座標y1
 * @param BMP 點陣圖形
 * @note  x0,y0 -- 起始點坐標(x0:0~127, y0:0~7)
 *        x1,y1 -- 起點對角線(結束點)的坐標(x1:1~128,y1:1~ 8)
 */
void SSD1306_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned char x, y;
    unsigned char *p;
    p = BMP;
    if (y1 % 4 == 0)
        y = y1 / 4;
    else
        y = y1 / 4 + 1;
    for (y = y0; y < y1; y++)
    {
        SSD1306_SetPos(x0, y);
        for (x = x0; x < x1; x++)
        {
            SSD1306_Write_Data(*p++);
        }
    }
}

/**
 * @brief 螢幕填充
 * @param BMP 點陣圖形
 */
void SSD1306_FILL(unsigned char BMP[])
{
    unsigned char i, j;
    unsigned char *p;
    p = BMP;

    for (i = 0; i < 8; i++)
    {
        SSD1306_Write_Cmd(0xB0 + i); //設置頁地址（0~7）
        SSD1306_Write_Cmd(0x00);     //設置顯示位置—列低地址
        SSD1306_Write_Cmd(0x10);     //設置顯示位置—列高地址

        for (j = 0; j < 128; j++)
        {
            SSD1306_Write_Data(*p++);
        }
    }
}

/**
 * @brief 螢幕清空
 */
void SSD1306_Clear(void)
{

#if (SCREEN_TYPE == SSD1306_128x32)
    page_len = 4;
#elif (SCREEN_TYPE == SSD1306_128x64)
    // p = ScreenBuffer[0];
    // page_len = 8;
#endif

    for (unsigned char i = 0; i < SCREEN_PAGE_NUM; i++)
    {
        SSD1306_Write_Cmd(0xB0 + i); //設置頁地址（0~7）
        SSD1306_Write_Cmd(0x00);     //設置顯示位置—列低地址
        SSD1306_Write_Cmd(0x10);     //設置顯示位置—列高地址

#define MODE (1)
#if (MODE == 1)

        for (unsigned char j = 0; j < 128; j++)
        {
            SSD1306_Write_Data(0x00);
        }
#elif (MODE == 2)
        SW_IIC_Start();
        SW_IIC_Write_Byte(SSD1306_ADDRESS);
        SW_IIC_WaitAck();
        SW_IIC_Write_Byte(0x40);
        SW_IIC_WaitAck();
        for (unsigned char n = 0; n < SCREEN_PAGEDATA_NUM; n++)
        {
            SW_IIC_Write_Byte(0x00);
            SW_IIC_WaitAck();
        }
        SW_IIC_Stop();
#endif
    }

    // unsigned char *p;
    //  SW_IIC_Start();
    //  SW_IIC_Write_Byte(SSD1306_ADDRESS);
    //  SW_IIC_WaitAck();
    //  SW_IIC_Write_Byte(0x40);
    //  SW_IIC_WaitAck();
    //  for (unsigned int n = 0; n < 128 * 9; n++)//原始*8 會被吃掉一的PAGE
    //  {
    //  		if( n%128 ==0)
    //  		{
    //  			__NOP;
    //  		}
    //      SW_IIC_Write_Byte(*p++);
    //      SW_IIC_WaitAck();
    //  }

    // SW_IIC_Stop();
}

/**
 * @brief 清空Page
 * @param page 一個Page
 * @note page: 0~7
 */
void SSD1306_Clear_Page(unsigned char page)
{
    unsigned char page_address_pos;
#if (SCREEN_TYPE == SSD1306_128x32)

    if (page >= 4)
        page = 3;
#elif (SCREEN_TYPE == SSD1306_128x64)
    if (page >= 7)
        page = 7;
#endif
    switch (page)
    {
    case 0:
        page_address_pos = 0;
        break;
    case 1:
        page_address_pos = 1;
        break;
    case 2:
        page_address_pos = 2;
        break;
    case 3:
        page_address_pos = 3;
        break;
    case 4:
        page_address_pos = 4;
        break;
    case 5:
        page_address_pos = 5;
        break;
    case 6:
        page_address_pos = 6;
        break;
    case 7:
        page_address_pos = 7;
        break;
    }

    SSD1306_Write_Cmd(0xB0 + page_address_pos); //設置頁地址（0~7）
    SSD1306_Write_Cmd(0x00);                    //設置顯示位置—列低地址
    SSD1306_Write_Cmd(0x10);                    //設置顯示位置—列高地址
    for (unsigned char n = 0; n < 128; n++)
        SSD1306_Write_Data(0x00);
}

/**
 * @brief 打開OLED
 */
void SSD1306_ON()
{
    SSD1306_Write_Cmd(0x8D);
    SSD1306_Write_Cmd(0x14);
    SSD1306_Write_Cmd(0xAF);
}

/**
 * @brief 關閉OLED
 */
void SSD1306_OFF()
{
    SSD1306_Write_Cmd(0x8D);
    SSD1306_Write_Cmd(0x10);
    SSD1306_Write_Cmd(0xAE);
}

void SSD1306_Run_Right()
{
    SSD1306_Write_Cmd(0x2e);

    SSD1306_Write_Cmd(0x29);
    SSD1306_Write_Cmd(0x00);
    SSD1306_Write_Cmd(0x00);
    SSD1306_Write_Cmd(0x06);
    SSD1306_Write_Cmd(0x07);

    SSD1306_Write_Cmd(0x2f);
}

void SSD1306_Vertical()
{
    SSD1306_Write_Cmd(0x2e);

    SSD1306_Write_Cmd(0xa3);
    SSD1306_Write_Cmd(0x00);
    SSD1306_Write_Cmd(0x40);

    SSD1306_Write_Cmd(0x2a);
    SSD1306_Write_Cmd(0x00);
    SSD1306_Write_Cmd(0x00);
    SSD1306_Write_Cmd(0x01);
    SSD1306_Write_Cmd(0x00);
    SSD1306_Write_Cmd(0x04);

    SSD1306_Write_Cmd(0x2f);
}
