#include <s3c2440.h>
#include "mmu.h"
#include "IRQ.h"
#include "sys.h"
#include "LCD.h"
#include "adc.h"
#include "gui.h"

/*******************************************************
 * 函数名称：main
 * 描述说明：主函数
 * 输入参数：无
 * 输出参数：无
*******************************************************/ 
int main()
{
	struct LcdDot TouchXY;
	// 系统时钟设置初始化
	Set_FCLK(400);				// 设置主频时钟FCLK=400MHz
	Set_ClkDivn(14,12);			// 设置FCLK:HCLK:PCLK=8:2:1

	IRQ_Init();
    Uart0_Init(115200);			// 串口初始化 波特率为115200
 	Lcd_Init();					// LCD初始化
	ADC_Init();					// 触摸屏初始化，等待触摸屏被按下

	Uart_Printf("********************************************************\n");
	Uart_Printf("*             福建师范大学现代显示技术                 *\n");
	Uart_Printf("*                  -LCD显示实验-                       *\n");
	Uart_Printf("*                   Version 1.0                        *\n");
	Uart_Printf("********************************************************\n");
	
//	AD2LCD = 1;				// 第一次进入系统，需进行触摸屏校正
//	Touch_to_LCD();
//	AD2LCD = 0;

ReFlash:
	while(1)
	{
		Paint_Bmp(0, 0, 240, 320, main_img);
		if(GetTouch(&TouchXY))
		{
			switch(GetIcon(&TouchXY))
			{
				case 1:
					Uart_Printf("选择难度'1'");
					PlayGame(1);
					goto ReFlash;
				case 2:
					Uart_Printf("选择难度'2'");
					PlayGame(2);
					goto ReFlash;
				case 3:
					Uart_Printf("选择难度'3'");
					PlayGame(3);
					goto ReFlash;
				case 4:
					Uart_Printf("选择难度'4'");
					PlayGame(4);
					goto ReFlash;
				default:
					break;			
			}
			Lcd_printf(TouchXY.x, TouchXY.y, RGB( 0xff,0x00,0x00),RGB( 0x00,0xff,0x00),0,"点"  );          
		}
	}			
}
