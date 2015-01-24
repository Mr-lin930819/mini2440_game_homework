#ifndef	IRQ_H_
#define	IRQ_H_

// �����ж�ƫ��ֵ
#define INT_ADC			31			// ADC�ж�
#define INT_RTC			30			// RTCʱ���ж�
#define INT_SPI1		29			// SPI1�ж�
#define INT_UART0		28			// UART0�ж�
#define INT_IIC			27			// IIC�ж�
#define INT_USBH		26			// USB�����ж�
#define INT_USBD		25			// USB�豸�ж�
#define INT_NFCON		24			// Nand Flash�����ж�
#define INT_UART1		23			// UART1�ж�
#define INT_SPI0		22			// SPI0�ж�
#define INT_SDI			21			// SDI�ж�
#define INT_DMA3		20			// DMAͨ��3�ж�
#define INT_DMA2		19			// DMAͨ��2�ж�
#define INT_DMA1		18			// DMAͨ��1�ж�
#define INT_DMA0		17			// DMAͨ��0�ж�
#define INT_LCD			16			// LCD�ж�
#define INT_UART2		15			// UART2�ж�
#define INT_TIMER4		14			// ��ʱ��4�ж�
#define INT_TIMER3		13			// ��ʱ��3�ж�
#define INT_TIMER2		12			// ��ʱ��2�ж�
#define INT_TIMER1		11			// ��ʱ��1�ж�
#define INT_TIMER0		10			// ��ʱ��0�ж�
#define INT_WDT_AC97	9			// AC97�����Ź��ж�
#define INT_TICK		8			// RTCʱ�ӵδ��ж�
#define nBATT_FLT		7			// ��ع����ж�
#define INT_CAM			6			// ����ͷ��׽�ж�
#define EINT8_23		5			// �ⲿ�ж�8-23
#define EINT4_7			4			// �ⲿ�ж�4-7
#define EINT3			3			// �ⲿ�ж�3
#define EINT2			2			// �ⲿ�ж�2
#define EINT1			1			// �ⲿ�ж�1
#define EINT0			0			// �ⲿ�ж�0


void HandlerIRQ(void);		//����IRQ�ж�
void IRQ_Init(void);		//�жϳ�ʼ��
void EINT8_Init(void);		//EINT8_23�ⲿ�жϳ�ʼ��

#endif
