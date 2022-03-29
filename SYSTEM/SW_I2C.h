#ifndef __SW_I2C_H__
#define __SW_I2C_H__

#include "M051Series.h"

#define SOFTIIC1 (1)
#define SOFTIIC2 (2)
#define SOFTIICPINCONFIG (SOFTIIC1)

#if (SOFTIICPINCONFIG == SOFTIIC1)

#define IIC_SCL P20 //CLK
#define IIC_SDA P21 //SDA
#define IIC_SDA_STATUS P21

#elif (SOFTIICPINCONFIG == SOFTIIC2)
#define IIC_SCL P30 //時脈
#define IIC_SDA P31 //資料
#define IIC_SDA_STATUS P31
#endif

#define IIC_SCL_L IIC_SCL = 0 //CLK_L
#define IIC_SCL_H IIC_SCL = 1 //CLK_H
#define IIC_SDA_L IIC_SDA = 0 //SDA_L
#define IIC_SDA_H IIC_SDA = 1 //SDA_H

/********************************************************************************/

/****************************************************************
函式庫列表
*****************************************************************/
void SW_IIC_GPIO_Config(void);
uint8_t SW_IIC_Start(void);
void SW_IIC_Stop(void);
void SW_IIC_Ack(void);
uint8_t SW_IIC_WaitAck(void);
void SW_IIC_Nack(void);
void SW_IIC_Write_Byte(unsigned char data);
unsigned char SW_IIC_Read_Byte(void);

#endif
