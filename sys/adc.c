#include <s3c2440.h>
#include <math.h>
#include <stdlib.h>
#include "sys.h"
#include "adc.h"
#include "LCD.h"
#include "gui.h"

//��������AD
uint32 xData,yData;
uint8 AD2LCD;				// �Ƿ���д�����У����1����ҪУ����0������ҪУ��
/*****************************************************************
 * �������ƣ�ADC_Init
 * ����˵����ADC��ʼ�������÷��жϷ�ʽ���ȴ�������������
 * �����������
 * �����������
*****************************************************************/
void ADC_Init(void)
{
	//ʹ��Ԥ��Ƶ���ܣ�����A/Dת������ʱ�� = PCLK/(49+1)
	ADCCON = PRESCALE_EN | PRSCVL(49);
	
	//������ʱʱ�� = (1/3.6864M)*50000 = 13.56ms
    //�����´��������ٹ�13.56ms�Ų���
    ADCDLY = 50000;

	//�ȴ��ж�ģʽ
	ADCTSC = 0xD3;
}

/*****************************************************************
 * �������ƣ�Get_AD_XY
 * ����˵������ȡ�������x,yת��ADֵ
 * �����������
 * ���������0-�޴�����1-��ȡ��������Ϣ�ɹ�����
*****************************************************************/
unsigned char Get_AD_XY( void )
{
	uint8 i;
    
	//ʹ��Ԥ��Ƶ���ܣ�����A/Dת������ʱ�� = PCLK/(49+1)
	ADCCON = PRESCALE_EN | PRSCVL(49);
	
	//������ʱʱ�� = (1/3.6864M)*50000 = 13.56ms
    //�����´��������ٹ�13.56ms�Ų���
    ADCDLY = 50000;

	//�ȴ��ж�ģʽ
	ADCTSC = 0xD3;

	// �޴������� 
	if( !(ADCUPDN & 1<<0) )
		return 0;						
	
	ADCUPDN= 0x00;				// ���жϱ�ʶ

	//�����Զ�(����) X/Y������ת��ģʽ
	ADCTSC = 0x0C; 

    //����A/Dת��
    ADCCON |= ADC_START;
	
	//��ѯ�ж�����״̬λ��ת��������ʼʱ����λ��0
	while(ADCCON & 0x1);
	//��ѯ�ж�ת��������־λ����ת����������λ��Ϊ1
    while(!(ADCCON & 0x8000));

	xData=(ADCDAT0&0x3ff);
	yData=(ADCDAT1&0x3ff);

	Uart_Printf("\nx_AD = %d, y_AD = %d\n", xData, yData);

	//�ȴ��û�����
	ADCTSC = 0xD3;    			// �ȴ��ж�ģʽ
	ADCTSC = ADCTSC|(1<<8); 	// �ȴ����������ɿ�

	for( i= 0; i< 1000; i++ )   // �жϴ������Ƿ��ɿ�
	{
		if( (ADCUPDN & 1<<1) )  
			break ;
	}

    ADCTSC = 0xD3;					// �ɼ���ɣ������жϵȴ�ģʽ  	
	ADCUPDN = 0x00 ;				// ���жϱ�ʶ

	return 1;
}
	   
/*****************************************************************
 * �������ƣ�Get_AD_XYs
 * ����˵����������ȡ�������x,yת��ADֵ
 * �����������
 * ���������0-�޴�����1-��ȡ��������Ϣ�ɹ�����
*****************************************************************/
unsigned char Get_AD_XYs( void )
{
	unsigned int i; 
	// �޴������� 
	if( !(ADCUPDN & 1<<0) )
	{
		return 0;
	}						
	
//	ADCUPDN= 0x00;				// ���жϱ�ʶ

	//�����Զ�(����) X/Y������ת��ģʽ
	ADCTSC = 0x0C; 

    //����A/Dת��
    ADCCON |= ADC_START;

	//��ѯ�ж�����״̬λ��ת��������ʼʱ����λ��0
	while(ADCCON & 0x1);
	//��ѯ�ж�ת��������־λ����ת����������λ��Ϊ1
    while(!(ADCCON & 0x8000));

	xData=(ADCDAT0&0x3ff);
	yData=(ADCDAT1&0x3ff);

	ADCTSC = 0xD3;    			// �ȴ��ж�ģʽ

	if((ADCDAT0 & 0x3ff)==0)
	{
		ADCUPDN= 0x00;
		Uart_Printf("Up\n");
		return 0;	
	}

//	Uart_Printf("Down:x_AD = %d, y_AD = %d\n", xData, yData);
	i = 0xFFFF;
	while (i--);

	return 1;
}

/*****************************************************************
 * �������ƣ�GetTouch
 * ����˵������ȡAD�����꣬ת��ΪLCD��������
 * ���������
 * ���������0-�޴������أ�1-����ֵת���ɹ�
*****************************************************************/
//У׼���ֵ
int	MIN_X = 91;
int	MAX_X = 929;
int	MIN_Y = 112;
int	MAX_Y = 929;
unsigned char GetTouch( struct LcdDot *lcdXY ) 
{
	//�޴���������
	if( !Get_AD_XY() )
		return 0;

	if( xData< MIN_X ) xData = MIN_X; 
	if( xData> MAX_X ) xData= MAX_X; 
	xData = 239*(xData-MIN_X)/(MAX_X- MIN_X); //LCD X ����ֵ
	xData = 239 - xData;

 	if( yData < MIN_Y) yData = MIN_Y;
 	if( yData > MAX_Y) yData = MAX_Y ;
 	
	yData= 319*(yData - MIN_Y)/(MAX_Y- MIN_Y);

    (*lcdXY).x= xData ;
    (*lcdXY).y= yData ;

//	Uart_Printf("xData = %d, yData = %d\n", xData, yData);

    return 1;
}

int x_min, x_max, y_min, y_max;						// AD�߽�ֵ

void Touch_to_LCD(void)
{
	int x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;		// ��βɼ�������ADֵ

	if(AD2LCD)
	{
		Lcd_ClearScr(RGB(0x00, 0x00, 0xff));		// ��Ļ���Ϊ��ɫ

		do
		{		
			// ��һ����ʮ����, (30, 30)
			GUI_DrawHLine(20, 30, 40,RGB(0xff,0x00,0x00));	// ��y=30λ�ô�����һ��ˮƽ��20<x<40
			GUI_DrawVLine(30, 20, 40,RGB(0xff,0x00,0x00));	// ��x=30λ�ô�����һ����ֱ��20<y<40
		
			while(!Get_AD_XY());
			x0 = xData;
			y0 = yData;
			Lcd_ClearScr(RGB(0x00, 0x00, 0xff));			// ��Ļ���Ϊ��ɫ
	
			// �ڶ�����ʮ����, (210, 30)
			GUI_DrawHLine(200, 30, 220,RGB(0xff,0x00,0x00));// ��y=30λ�ô�����һ��ˮƽ��200<x<220
			GUI_DrawVLine(210, 20, 40,RGB(0xff,0x00,0x00));	// ��x=210λ�ô�����һ����ֱ��20<y<40
	
			while(!Get_AD_XY());
			x1 = xData;
			y1 = yData;
			Lcd_ClearScr(RGB(0x00, 0x00, 0xff));			// ��Ļ���Ϊ��ɫ
				
			// ��������ʮ����, (30, 290)
			GUI_DrawHLine(20, 290, 40,RGB(0xff,0x00,0x00));	// ��y=290λ�ô�����һ��ˮƽ��20<x<40
			GUI_DrawVLine(30, 280, 300,RGB(0xff,0x00,0x00));// ��x=30λ�ô�����һ����ֱ��20<y<40
	
			while(!Get_AD_XY());
			x2 = xData;
			y2 = yData;
			Lcd_ClearScr(RGB(0x00, 0x00, 0xff));			// ��Ļ���Ϊ��ɫ
				
			// ���ĸ���ʮ����, (210, 290)
			GUI_DrawHLine(200, 290, 220,RGB(0xff,0x00,0x00));// ��y=290λ�ô�����һ��ˮƽ��20<x<40
			GUI_DrawVLine(210, 280, 300,RGB(0xff,0x00,0x00));// ��x=30λ�ô�����һ����ֱ��20<y<40
	
			while(!Get_AD_XY());
			x3 = xData;
			y3 = yData;
			Lcd_ClearScr(RGB(0x00, 0x00, 0xff));			// ��Ļ���Ϊ��ɫ
				
			// �������ʮ����, (120, 160)
			GUI_DrawHLine(110, 160, 130,RGB(0xff,0x00,0x00));	// ��y=290λ�ô�����һ��ˮƽ��20<x<40
			GUI_DrawVLine(120, 150, 170,RGB(0xff,0x00,0x00));// ��x=30λ�ô�����һ����ֱ��20<y<40
	
			while(!Get_AD_XY());
			x4 = xData;
			y4 = yData;
			Lcd_ClearScr(RGB(0x00, 0x00, 0xff));			// ��Ļ���Ϊ��ɫ
	
			// ͨ���ĵ����꣬����ƽ��ֵ
			x_min = (x0+x2)/2;
			x_max = (x1+x3)/2;
			y_min = (y0+y1)/2;
			y_max = (y2+y3)/2;
					
//			Uart_Printf("(%d,%d), (%d,%d), (%d,%d), (%d,%d), (%d,%d)\n", x0, y0, x1, y1, x2, y2, x3, y3, x4, y4);	
//			Uart_Printf("4��ƽ��ֵ��(%d,%d), (%d,%d), (%d,%d), (%d,%d)\n", x_min, y_min, x_max, y_min, x_min, y_max, x_max, y_max);	
	
		}while(abs(x0-x2)>12||abs(x1-x3)>12||abs(y0-y1)>12||abs(y2-y3)>12||abs(x_min+x_max-x4*2)>20||abs(y_min+y_max-y4*2)>20);
	}
}

unsigned char Get_Touch( struct LcdDot *lcdXY ) 
{
	//�޴���������
	if( !Get_AD_XY() )
		return 0;

	(*lcdXY).x = 180*(int)(xData - x_min)/(int)(x_max - x_min)+30;
	(*lcdXY).y = 260*(int)(yData - y_min)/(int)(y_max - y_min)+30;

//	Uart_Printf("xData = %d, yData = %d\n", (*lcdXY).x, (*lcdXY).y);

    return 1;
}
