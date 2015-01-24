#include <s3c2440.h>
#include "IRQ.h"
#include "sys.h"
#include "adc.h"

void Key_LED(void)
{
	if(EINTPEND & (1<<8))	//ͨ���ⲿ�жϹ���Ĵ����ж����ĸ����������ж�
	{
		EINTPEND |= 1<<8;	//���ⲿ�жϹ���Ĵ�������ֹ�����ж�
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
 * �������ƣ�Timer0_Handle
 * ����˵������ʱ��0�жϴ�������LoopValÿ�仯һ�Σ�LED��״̬�����仯
 * �����������
 * �����������
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
 * �������ƣ�HandlerIRQ
 * ����˵��������IRQ�ж�
 * �����������
 * �����������
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
	ClearPending(1<<INTOFFSET); 		//���жϹ���Ĵ�������SRCPND INTPND
}


/*****************************************************************
 * �������ƣ�IRQ_Init
 * ����˵�����жϳ�ʼ��
 * �����������
 * �����������
*****************************************************************/
void IRQ_Init(void)
{
	INTMOD = 0x0;	      //�����ж϶���ΪIRQģʽ
	INTMSK = BIT_ALLMSK;  //���������ж�
}

/*****************************************************************
 * �������ƣ�EINT8_Init
 * ����˵������ʼ��GPG����Ϊ�ⲿ�жϣ�ʹ��Ĭ�ϵ͵�ƽ������IRQ��ʽ
 * �����������
 * �����������
*****************************************************************/
void EINT8_Init(void)
{
	// GPG11,7,6,5,3,0����Ϊ�ⲿ�ж�����,�ֱ��ӦK1~K6����
	GPGCON &= (~((3<<22)|(3<<14)|(3<<12)|(3<<10)|(3<<6)|(3<<0)));
	GPGCON |= ((2<<22)|(2<<14)|(2<<12)|(2<<10)|(2<<6)|(2<<0));

	EINTPEND |= ((1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19));		//���ⲿ�жϹ���Ĵ�������ֹ����
	EINTMASK &= ~((1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19));		//���ⲿ�ж�����

	ClearPending(BIT_EINT8_23);
	INTMSK &= ~(BIT_EINT8_23); 							//����EINT8-23���ж�����
}
