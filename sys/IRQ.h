#ifndef	IRQ_H_
#define	IRQ_H_

// 定义中断偏移值
#define INT_ADC			31			// ADC中断
#define INT_RTC			30			// RTC时钟中断
#define INT_SPI1		29			// SPI1中断
#define INT_UART0		28			// UART0中断
#define INT_IIC			27			// IIC中断
#define INT_USBH		26			// USB主机中断
#define INT_USBD		25			// USB设备中断
#define INT_NFCON		24			// Nand Flash控制中断
#define INT_UART1		23			// UART1中断
#define INT_SPI0		22			// SPI0中断
#define INT_SDI			21			// SDI中断
#define INT_DMA3		20			// DMA通道3中断
#define INT_DMA2		19			// DMA通道2中断
#define INT_DMA1		18			// DMA通道1中断
#define INT_DMA0		17			// DMA通道0中断
#define INT_LCD			16			// LCD中断
#define INT_UART2		15			// UART2中断
#define INT_TIMER4		14			// 定时器4中断
#define INT_TIMER3		13			// 定时器3中断
#define INT_TIMER2		12			// 定时器2中断
#define INT_TIMER1		11			// 定时器1中断
#define INT_TIMER0		10			// 定时器0中断
#define INT_WDT_AC97	9			// AC97及看门狗中断
#define INT_TICK		8			// RTC时钟滴答中断
#define nBATT_FLT		7			// 电池故障中断
#define INT_CAM			6			// 摄像头捕捉中断
#define EINT8_23		5			// 外部中断8-23
#define EINT4_7			4			// 外部中断4-7
#define EINT3			3			// 外部中断3
#define EINT2			2			// 外部中断2
#define EINT1			1			// 外部中断1
#define EINT0			0			// 外部中断0


void HandlerIRQ(void);		//处理IRQ中断
void IRQ_Init(void);		//中断初始化
void EINT8_Init(void);		//EINT8_23外部中断初始化

#endif
