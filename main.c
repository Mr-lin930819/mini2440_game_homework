#include <s3c2440.h>
#include "mmu.h"
#include "IRQ.h"
#include "sys.h"
#include "LCD.h"
#include "adc.h"
#include "gui.h"

/*******************************************************
 * �������ƣ�main
 * ����˵����������
 * �����������
 * �����������
*******************************************************/ 
int main()
{
	struct LcdDot TouchXY;
	// ϵͳʱ�����ó�ʼ��
	Set_FCLK(400);				// ������Ƶʱ��FCLK=400MHz
	Set_ClkDivn(14,12);			// ����FCLK:HCLK:PCLK=8:2:1

	IRQ_Init();
    Uart0_Init(115200);			// ���ڳ�ʼ�� ������Ϊ115200
 	Lcd_Init();					// LCD��ʼ��
	ADC_Init();					// ��������ʼ�����ȴ�������������

	Uart_Printf("********************************************************\n");
	Uart_Printf("*             ����ʦ����ѧ�ִ���ʾ����                 *\n");
	Uart_Printf("*                  -LCD��ʾʵ��-                       *\n");
	Uart_Printf("*                   Version 1.0                        *\n");
	Uart_Printf("********************************************************\n");
	
//	AD2LCD = 1;				// ��һ�ν���ϵͳ������д�����У��
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
					Uart_Printf("ѡ���Ѷ�'1'");
					PlayGame(1);
					goto ReFlash;
				case 2:
					Uart_Printf("ѡ���Ѷ�'2'");
					PlayGame(2);
					goto ReFlash;
				case 3:
					Uart_Printf("ѡ���Ѷ�'3'");
					PlayGame(3);
					goto ReFlash;
				case 4:
					Uart_Printf("ѡ���Ѷ�'4'");
					PlayGame(4);
					goto ReFlash;
				default:
					break;			
			}
			Lcd_printf(TouchXY.x, TouchXY.y, RGB( 0xff,0x00,0x00),RGB( 0x00,0xff,0x00),0,"��"  );          
		}
	}			
}
