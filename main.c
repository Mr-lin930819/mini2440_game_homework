#include <s3c2440.h>
#include "mmu.h"
#include "IRQ.h"
#include "sys.h"
#include "LCD.h"
#include "adc.h"
#include "gui.h"

/*******************************************************
 * ????:main
 * ????:???
 * ????:?
 * ????:?
*******************************************************/ 
int main()
{
	struct LcdDot TouchXY;
	// ?????????
	Set_FCLK(400);				// ??????FCLK=400MHz
	Set_ClkDivn(14,12);			// ??FCLK:HCLK:PCLK=8:2:1

	IRQ_Init();
    Uart0_Init(115200);			// ????? ????115200
 	Lcd_Init();					// LCD???
	ADC_Init();					// ??????,????????

	Uart_Printf("********************************************************\n");
	Uart_Printf("*             ????????????                 *\n");
	Uart_Printf("*                  -LCD????-                       *\n");
	Uart_Printf("*                   Version 1.0                        *\n");
	Uart_Printf("********************************************************\n");
	
//	AD2LCD = 1;				// ???????,????????
//	Touch_to_LCD();
//	AD2LCD = 0;

ReFlash:
	while(1)
	{
		Paint_Bmp(0, 0, 240, 320, game_level_bg);
		//Paint_Bmp(0, 0, 240, 320, game_pass);
		//Paint_Bmp(0, 0, 240, 320, game_scene_ui);
		//Paint_Bmp(0, 0, 240, 320, game_pp);
		if(GetTouch(&TouchXY))
		{
			Uart_Printf("%d  ,  %d",TouchXY.x,TouchXY.y);
			switch(GetIcon(&TouchXY))
			{
				case 1:
					Uart_Printf("ѡ���Ѷ�'1'");
					PlayGame(1);
					//LED_Test();			// LED???
					goto ReFlash;
				case 2:
					Uart_Printf("?????'2'");
					PlayGame(2);
					//Lcd_Test();			// ??????
					goto ReFlash;
				case 3:
					Uart_Printf("?????'3'");
					PlayGame(3);
					//AD2LCD = 1;
					//Touch_to_LCD();		// ????
					goto ReFlash;
				case 4:
					Uart_Printf("?????'4'");
					PlayGame(4);
					goto ReFlash;
				/*
				case 5:
					Uart_Printf("?????'5'");
					//PY_Test();			// ????
					goto ReFlash;
				case 6:
					Uart_Printf("?????'6'");
					goto ReFlash;
				case 7:
					Uart_Printf("?????'7'");
					goto ReFlash;					
				case 8:
					Uart_Printf("?????'8'");
					goto ReFlash;
				case 9:
					Uart_Printf("?????'9'");
					goto ReFlash;
				*/
				default:
					break;			
			}
			//Lcd_printf(TouchXY.x, TouchXY.y, RGB( 0xff,0x00,0x00),RGB( 0x00,0xff,0x00),0,"?"  );          
		}
	}			
}



/*******************************************************
 * �������ƣ�main
 * ����˵����������
 * �����������
 * �����������
********************************************************/

/*
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
		Paint_Bmp(0, 0, 240, 320, game_level_bg);
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
*/
