#include <s3c2440.h>
#include "sys.h"

/*****************************************************************
 * 函数名称：Timer0Init
 * 描述说明：定时器0初始化,设置Timer0每隔1s触发一次中断
 * 输入参数：无
 * 输出参数：无
*****************************************************************/
void Timer0Init(void)
{
	/*
	// 定时器设置
	TCFG0 &= 0xff00;			// 清除Timer0&1的预分频器0
	TCFG0 |= 99;				// 设置预分频器0=15

	TCFG1 &= 0xffff0;			// 清除Timer 0的分频值
	TCFG1 |= 0x00002;			// 设置Timer 0为8分频
	
	//初始计数值=Timer0时钟频率，即1s触发一次中断
    TCNTB0 = PCLK*1000000/100/8;	

    TCON = TCON | 0x02;		// 手动更新，将TCNTBn的值载入TCNTn	
	TCON = 0x09; 			// 自动加载，启动定时器0
	ClearPending(BIT_TIMER0);
	INTMSK &= ~(BIT_TIMER0);
	*/
	TCFG0&=~(0xff);
	TCFG0|=99;
   TCFG1&=~(0xf);
		TCFG1|=0x02;
   TCNTB0=62500;
   TCON|=(1<<1);
   TCON=0x09;
	INTMSK&=~(1<<10);
}
