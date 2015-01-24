#ifndef _SYS_H
#define _SYS_H

#define uint8 unsigned char
#define int8 char
#define uint16 unsigned short
#define int32 int
#define uint32 unsigned int

extern unsigned long LoopVal;
//单位MHz
extern uint32 FCLK;
extern uint32 HCLK;
extern uint32 PCLK;

//延时函数
void delay(unsigned int t);	

//寄存器位操作, 置位或清零
//reg:寄存器, x:要置位或清零位
#define	SET_REG(reg, x)		(reg |=  (1<<(x)))
#define	CLR_REG(reg, x)		(reg &= ~(1<<(x)))

//系统时钟相关函数
void Set_FCLK(uint32 fclk);						//设置主频时钟
void Set_ClkDivn(uint32 hDivn, uint32 pDivn);	//设置FCLK:HCLK:PCLK比例


//串口输出显示相关函数
void Uart0_Init(uint32 baud);					// 串口初始化
void Uart_Printf(const char *fmt, ...);		// 串口格式化输出
uint8 Uart_GetByte(void);					// 从串口读取一个字符

//LED输出显示函数
void Led_Out(int data);

//定时器0初始化
void Timer0Init(void);


#endif
