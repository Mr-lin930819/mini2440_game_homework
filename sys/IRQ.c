#include <s3c2440.h>
#include "IRQ.h"
#include "sys.h"
#include "adc.h"

void Key_LED(void)
{
	if(EINTPEND & (1<<8))	//通过外部中断挂起寄存器判断是哪个按键请求中断
	{
		EINTPEND |= 1<<8;	//清外部中断挂起寄存器，防止反复中断
		Led_Out(0x01);
	}
	if(EINTPEND & (1<<11))
	{
		EINTPEND |= 1<<11;
		Led_Out(0x02);
	}
	if(EINTPEND & (1<<13))
	{
		EINTPEND |= 1<<13;
		Led_Out(0x04);
	}
	if(EINTPEND & (1<<14))
	{
		EINTPEND |= 1<<14;
		Led_Out(0x08);
	}
	if(EINTPEND & (1<<15))
	{
		EINTPEND |= 1<<15;
		Led_Out(0x0f);
	}
	if(EINTPEND & (1<<19))
	{
		EINTPEND |= 1<<19;
		Led_Out(0x00);
	}
}

/*****************************************************************
 * 函数名称：Timer0_Handle
 * 描述说明：定时器0中断处理函数，LoopVal每变化一次，LED灯状态发生变化
 * 输入参数：无
 * 输出参数：无
*****************************************************************/
unsigned long LoopVal;
void Timer0_Handle(void)
{
	LoopVal++;
	if(LoopVal%2 == 0)
		Led_Out(0x00);
	else
		Led_Out(0x05);
}

/*****************************************************************
 * 函数名称：HandlerIRQ
 * 描述说明：处理IRQ中断
 * 输入参数：无
 * 输出参数：无
*****************************************************************/
void HandlerIRQ(void)
{
	switch(INTOFFSET)
	{
/*		case INT_ADC:
		case INT_RTC:
		case INT_SPI1:
		case INT_UART0:
		case INT_IIC:
		case INT_USBH:
		case INT_USBD:
		case INT_NFCON:
		case INT_UART1:
		case INT_SPI0:
		case INT_SDI:
		case INT_DMA3:
		case INT_DMA2:
		case INT_DMA1:
		case INT_DMA0:
		case INT_LCD:
		case INT_UART2:
		case INT_TIMER4:
		case INT_TIMER3:
		case INT_TIMER2:
		case INT_TIMER1:
*/
		case INT_TIMER0:
			Timer0_Handle();
			break;
/*
		case INT_WDT_AC97: 
		case INT_TICK:
		case nBATT_FLT:
		case INT_CAM:	
*/
		case EINT8_23:
			Key_LED();
			break;	 
//		case EINT4_7:	
//		case EINT3:
//		case EINT2:
//		case EINT1:
//		case EINT0:
  		default:
			break;
	}
	ClearPending(1<<INTOFFSET); 		//清中断挂起寄存器包括SRCPND INTPND
}


/*****************************************************************
 * 函数名称：IRQ_Init
 * 描述说明：中断初始化
 * 输入参数：无
 * 输出参数：无
*****************************************************************/
void IRQ_Init(void)
{
	INTMOD = 0x0;	      //所有中断都设为IRQ模式
	INTMSK = BIT_ALLMSK;  //屏蔽所有中断
}

/*****************************************************************
 * 函数名称：EINT8_Init
 * 描述说明：初始化GPG引脚为外部中断，使用默认低电平触发，IRQ方式
 * 输入参数：无
 * 输出参数：无
*****************************************************************/
void EINT8_Init(void)
{
	// GPG11,7,6,5,3,0配置为外部中断属性,分别对应K1~K6按键
	GPGCON &= (~((3<<22)|(3<<14)|(3<<12)|(3<<10)|(3<<6)|(3<<0)));
	GPGCON |= ((2<<22)|(2<<14)|(2<<12)|(2<<10)|(2<<6)|(2<<0));

	EINTPEND |= ((1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19));		//清外部中断挂起寄存器，防止干扰
	EINTMASK &= ~((1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19));		//打开外部中断请求

	ClearPending(BIT_EINT8_23);
	INTMSK &= ~(BIT_EINT8_23); 							//开启EINT8-23的中断请求
}
