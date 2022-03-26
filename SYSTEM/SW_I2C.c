#include "SW_I2C.h"

/**
 * @brief  開始
 * @return uint8_t
 * 			0 -- 成功
 * 			1 -- 錯誤
 */
uint8_t SW_IIC_Start(void)
{
	IIC_SDA_H;
	IIC_SCL_H;
	// CLK_SysTickDelay(1);
	if (!IIC_SDA_STATUS)
		return 1; // SDA為低電位,忙碌中,退出
	IIC_SDA_L;
	// CLK_SysTickDelay(1);
	if (IIC_SDA_STATUS)
		return 1; // SDA為高電位,出錯,退出
	IIC_SCL_L;
	// CLK_SysTickDelay(1);
	return 0;
}

/**
 * @brief 停止
 */
void SW_IIC_Stop(void)
{
	IIC_SCL_L;
	IIC_SDA_L;
	// CLK_SysTickDelay(1);
	IIC_SCL_H;
	// CLK_SysTickDelay(1);
	IIC_SDA_H;
	// CLK_SysTickDelay(1);
}

/**
 * @brief 無應答信號
 */
void SW_IIC_Nack(void)
{
	IIC_SCL_L;
	IIC_SDA_H;
	// CLK_SysTickDelay(1);
	IIC_SCL_H;
	// CLK_SysTickDelay(1);
	IIC_SCL_L;
}

/**
 * @brief 應答信號
 */
void SW_IIC_Ack(void)
{
	IIC_SCL_L;
	// CLK_SysTickDelay(1);
	IIC_SDA_L;
	// CLK_SysTickDelay(1);
	IIC_SCL_H;
	// CLK_SysTickDelay(1);
	IIC_SCL_L;
	// CLK_SysTickDelay(1);
}

/**
 * @brief 等待應答信號
 * @return uint8_t
 * 			0 -- 成功
 * 			1 -- 錯誤
 */
uint8_t SW_IIC_WaitAck(void)
{
	uint8_t Err_count = 0;

	IIC_SCL_L;
	// CLK_SysTickDelay(1);
	IIC_SDA_H;
	// CLK_SysTickDelay(1); //發送完後釋放資料線，準備接收應答位
	IIC_SCL_H;
	// CLK_SysTickDelay(1);

	while (IIC_SDA_STATUS)
	{
		Err_count++;
		if (Err_count > 250)
		{
			SW_IIC_Stop();
			// IIC_SDA = 0;//退出
			return 1;
		}
	}
	IIC_SCL_L;
	return 0;
}

/**
 * @brief 寫入一個Byte的資料
 * @param data 一個Byte資料
 */
void SW_IIC_Write_Byte(unsigned char data)
{
	IIC_SCL_L; //拉低開始傳資料
	for (unsigned char count = 0; count < 8; count++)
	{
		if ((data & 0x80) >> 7)
			IIC_SDA_H;
		else
			IIC_SDA_L;
		data <<= 1;
		// CLK_SysTickDelay(1);
		IIC_SCL_H;
		// CLK_SysTickDelay(1);
		IIC_SCL_L;
		// CLK_SysTickDelay(1);
	}
}

/**
 * @brief 讀取一個Byte的資料
 * @return unsigned char 讀取的資料
 * @note 從高位到低位
 */
unsigned char SW_IIC_Read_Byte(void)
{
	unsigned char count, data = 0;
	for (count = 0; count < 8; count++)
	{
		IIC_SCL_L;
		// CLK_SysTickDelay(1);
		IIC_SCL_H;
		data <<= 1;
		// data=(data<<1)|IIC_SDA;
		if (IIC_SDA_STATUS == 1)
			data |= 0X01;
		// IIC_SCL_H;
		//  CLK_SysTickDelay(1);
		IIC_SCL_L;
		// CLK_SysTickDelay(1);
	}
	return data;
}

void SW_IIC_GPIO_Config(void)
{
#if (SOFTIICPINCONFIG == SOFTIIC1)
	GPIO_SetMode(P2, BIT0, GPIO_PMD_QUASI);
	GPIO_SetMode(P2, BIT1, GPIO_PMD_QUASI);
#elif (SOFTIICPINCONFIG == SOFTIIC2)
	GPIO_SetMode(P3, BIT0, GPIO_PMD_QUASI);
	GPIO_SetMode(P3, BIT1, GPIO_PMD_QUASI);
#endif
}
