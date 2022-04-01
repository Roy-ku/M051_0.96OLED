#include "OLED_SSD1306_Driver.h"
///////////////////////////////////////////////////////

#if (TRANSFER_METHOD == HW_IIC)

uint8_t HW_I2C_Continuous_WriteByte(I2C_T *i2c, uint8_t data)
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
            I2C_SET_DATA(i2c, 0x40);        /* Write data */
            u8Ctrl = I2C_I2CON_SI;          /* Clear SI */
            break;
        /*發送數據成功並收到ACK*/
        case 0x28:
            if (u8length < SCREEN_PAGEDATA_NUM)
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

uint8_t HW_I2C_WriteByte(I2C_T *i2c, uint8_t controlAddr, uint8_t data)
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

void SSD1306_Write_Cmd(uint8_t data)
{
    //SSD1306_Write(0x00,data);
    HW_I2C_WriteByte(I2CX, 0x00, data);
}

void SSD1306_Write_Data(uint8_t data)
{
    //SSD1306_Write(0x40,data);
    HW_I2C_WriteByte(I2CX, 0x40, data);
}

#elif (TRANSFER_METHOD == SW_IIC)

void SSD1306_Continuous_Write(uint8_t data)
{
    SW_IIC_Start();
    SW_IIC_Write_Byte(SSD1306_ADDRESS);
    SW_IIC_WaitAck();
    SW_IIC_Write_Byte(0x40);
    SW_IIC_WaitAck();
    for (uint8_t n = 0; n < SCREEN_PAGEDATA_NUM; n++)
    {
        SW_IIC_Write_Byte(data);
        SW_IIC_WaitAck();
    }
    SW_IIC_Stop();
}

void SSD1306_Write(uint8_t controladdr, uint8_t data)
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

void SSD1306_Write_Cmd(uint8_t data)
{
    SSD1306_Write(0x00, data);
}

void SSD1306_Write_Data(uint8_t data)
{
    SSD1306_Write(0x40, data);
}

#endif

/**
 * @brief 初始化設定
 */
void SSD1306_Init(void)
{
/*GPIO初始化*/
#if (TRANSFER_METHOD == HW_IIC)
    IIC_Configuration();
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
    /* Enable I2C0 module clock */
    CLK_EnableModuleClock(I2C0_MODULE);
    /* Configure the SDA0 & SCL0 of I2C0 pins */
    SYS->P3_MFP &= ~(SYS_MFP_P34_Msk | SYS_MFP_P35_Msk);
    SYS->P3_MFP |= (SYS_MFP_P34_SDA0 | SYS_MFP_P35_SCL0);
    /* Open I2C0 module and set bus clock */
    I2C_Open(I2CX, IIC_SPEED);
    /* Get I2C0 Bus Clock */
    // printf("I2C0 clock %d Hz\n", I2C_GetBusClockFreq(I2C0));
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
