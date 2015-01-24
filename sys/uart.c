#include <s3c2440.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "sys.h"

/*****************************************************************
 * 函 数 名：Uart0_Init
 * 功能描述：串口0初始化
 * 输入参数：baud：波特率
 * 返 回 值：无
 *****************************************************************/
void Uart0_Init(uint32 baud)
{
	uint32 UART_CLK;
	uint32 i;
	if (PCLK == 0)
	{
		Set_FCLK(400);				// 设置主频时钟FCLK=400MHz
		Set_ClkDivn(14,12);			// 设置FCLK:HCLK:PCLK=8:2:1
	}
	//GPH3用作RXD0
	SET_REG(GPHCON, 7);
	CLR_REG(GPHCON, 6);

	//GPH2用作TXD0
	SET_REG(GPHCON, 5);
	CLR_REG(GPHCON, 4);

	//选择UART时钟源、设置UART中断方式
	UCON0 = 0x05;		//中断方式或查询方式，时钟源PCLK

	//设置波特率
	UART_CLK = PCLK * 1000000;		//时钟源PCLK
	UBRDIV0 = ( (uint32)(UART_CLK/16.0/baud+0.5) -1 );

	//设置传输格式为:数据宽度8位，1位停止位，无奇偶检验位
    ULCON0 = 0x03;

    UFCON0 = 0x00;   	//不使用FIFO
	UMCON0 = 0x00;   	//不使用流控
	for(i=0; i<100; i++);
}


/*****************************************************************
 * 函 数 名：Uart_SendByte
 * 功能描述：串口发送一个字符
 * 输入参数：data--要发送的字符
 * 返 回 值：无
*****************************************************************/
void Uart_SendByte(char data)
{
    while(!(UTRSTAT0 & 0x2));   	//等待缓冲区数据发送完

    UTXH0 = data;					//向UTXH0写数据，UART自动将它发送出去
}


/*****************************************************************
 * 函 数 名：Uart_SentStr
 * 功能描述：从串口输出一个字符串
 * 输入参数：str--字符串数组首地址
 * 返 回 值：无
*****************************************************************/
void Uart_SentStr(char str[])
{
	int i;

	for (i=0; str[i]!='\0'; i++)
	{
		Uart_SendByte(str[i]);
	}
} 


/*****************************************************************
 * 函 数 名：Uart_GetByte
 * 功能描述：从串口读取一个字符
 * 输入参数：无
 * 返 回 值：读取到的字符
*****************************************************************/
uint8 Uart_GetByte(void)
{
    while(!(UTRSTAT0 & 0x1)); 	//等待数据接收完

	return URXH0; 
}


/*****************************************************************
 * 函 数 名：Uart_GetStr
 * 功能描述：从串口读取一个字符串
 * 输入参数：str--存放读取到的字符串
 * 返 回 值：无
*****************************************************************/
void Uart_GetStr(uint8 str[])
{
	uint8 c;

	while ( (c=Uart_GetByte()) != '\r' )
	{
        *str++ = c;
        Uart_SendByte(c);
	}

	*str = '\0';
}


/*****************************************************************
 * 函 数 名：Uart_Printf
 * 功能描述：串口格式化输出
 * 输入参数：变参
 * 返 回 值：无
*****************************************************************/
void Uart_Printf(const char *fmt, ...)
{
	va_list ap;
	char str[100];
	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	Uart_SentStr(str);
	va_end(ap);
}
