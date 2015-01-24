#include <s3c2440.h>
#include <math.h>
#include <stdlib.h>
#include "sys.h"
#include "adc.h"
#include "LCD.h"
#include "gui.h"

//触摸坐标AD
uint32 xData,yData;
uint8 AD2LCD;				// 是否进行触摸屏校正，1：需要校正，0：不需要校正
/*****************************************************************
 * 函数名称：ADC_Init
 * 描述说明：ADC初始化，采用非中断方式，等待触摸屏被按下
 * 输入参数：无
 * 输出参数：无
*****************************************************************/
void ADC_Init(void)
{
	//使能预分频功能，设置A/D转换器的时钟 = PCLK/(49+1)
	ADCCON = PRESCALE_EN | PRSCVL(49);
	
	//采样延时时间 = (1/3.6864M)*50000 = 13.56ms
    //即按下触摸屏后，再过13.56ms才采样
    ADCDLY = 50000;

	//等待中断模式
	ADCTSC = 0xD3;
}

/*****************************************************************
 * 函数名称：Get_AD_XY
 * 描述说明：获取触摸点的x,y转换AD值
 * 输入参数：无
 * 输出参数：0-无触摸；1-获取触摸点信息成功返回
*****************************************************************/
unsigned char Get_AD_XY( void )
{
	uint8 i;
    
	//使能预分频功能，设置A/D转换器的时钟 = PCLK/(49+1)
	ADCCON = PRESCALE_EN | PRSCVL(49);
	
	//采样延时时间 = (1/3.6864M)*50000 = 13.56ms
    //即按下触摸屏后，再过13.56ms才采样
    ADCDLY = 50000;

	//等待中断模式
	ADCTSC = 0xD3;

	// 无触摸返回 
	if( !(ADCUPDN & 1<<0) )
		return 0;						
	
	ADCUPDN= 0x00;				// 清中断标识

	//进入自动(连续) X/Y轴坐标转换模式
	ADCTSC = 0x0C; 

    //启动A/D转换
    ADCCON |= ADC_START;
	
	//查询中断启动状态位，转换真正开始时，该位清0
	while(ADCCON & 0x1);
	//查询中断转换结束标志位，当转换结束，该位置为1
    while(!(ADCCON & 0x8000));

	xData=(ADCDAT0&0x3ff);
	yData=(ADCDAT1&0x3ff);

	Uart_Printf("\nx_AD = %d, y_AD = %d\n", xData, yData);

	//等待用户松手
	ADCTSC = 0xD3;    			// 等待中断模式
	ADCTSC = ADCTSC|(1<<8); 	// 等待触摸屏被松开

	for( i= 0; i< 1000; i++ )   // 判断触摸屏是否松开
	{
		if( (ADCUPDN & 1<<1) )  
			break ;
	}

    ADCTSC = 0xD3;					// 采集完成，进入中断等待模式  	
	ADCUPDN = 0x00 ;				// 清中断标识

	return 1;
}
	   
/*****************************************************************
 * 函数名称：Get_AD_XYs
 * 描述说明：连续获取触摸点的x,y转换AD值
 * 输入参数：无
 * 输出参数：0-无触摸；1-获取触摸点信息成功返回
*****************************************************************/
unsigned char Get_AD_XYs( void )
{
	unsigned int i; 
	// 无触摸返回 
	if( !(ADCUPDN & 1<<0) )
	{
		return 0;
	}						
	
//	ADCUPDN= 0x00;				// 清中断标识

	//进入自动(连续) X/Y轴坐标转换模式
	ADCTSC = 0x0C; 

    //启动A/D转换
    ADCCON |= ADC_START;

	//查询中断启动状态位，转换真正开始时，该位清0
	while(ADCCON & 0x1);
	//查询中断转换结束标志位，当转换结束，该位置为1
    while(!(ADCCON & 0x8000));

	xData=(ADCDAT0&0x3ff);
	yData=(ADCDAT1&0x3ff);

	ADCTSC = 0xD3;    			// 等待中断模式

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
 * 函数名称：GetTouch
 * 描述说明：读取AD的坐标，转换为LCD像素坐标
 * 输入参数：
 * 输出参数：0-无触摸返回；1-坐标值转换成功
*****************************************************************/
//校准误差值
int	MIN_X = 91;
int	MAX_X = 929;
int	MIN_Y = 112;
int	MAX_Y = 929;
unsigned char GetTouch( struct LcdDot *lcdXY ) 
{
	//无触摸，返回
	if( !Get_AD_XY() )
		return 0;

	if( xData< MIN_X ) xData = MIN_X; 
	if( xData> MAX_X ) xData= MAX_X; 
	xData = 239*(xData-MIN_X)/(MAX_X- MIN_X); //LCD X 像素值
	xData = 239 - xData;

 	if( yData < MIN_Y) yData = MIN_Y;
 	if( yData > MAX_Y) yData = MAX_Y ;
 	
	yData= 319*(yData - MIN_Y)/(MAX_Y- MIN_Y);

    (*lcdXY).x= xData ;
    (*lcdXY).y= yData ;

//	Uart_Printf("xData = %d, yData = %d\n", xData, yData);

    return 1;
}

int x_min, x_max, y_min, y_max;						// AD边界值

void Touch_to_LCD(void)
{
	int x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;		// 五次采集的坐标AD值

	if(AD2LCD)
	{
		Lcd_ClearScr(RGB(0x00, 0x00, 0xff));		// 屏幕清空为蓝色

		do
		{		
			// 第一个“十”字, (30, 30)
			GUI_DrawHLine(20, 30, 40,RGB(0xff,0x00,0x00));	// 在y=30位置处，画一条水平线20<x<40
			GUI_DrawVLine(30, 20, 40,RGB(0xff,0x00,0x00));	// 在x=30位置处，画一条垂直线20<y<40
		
			while(!Get_AD_XY());
			x0 = xData;
			y0 = yData;
			Lcd_ClearScr(RGB(0x00, 0x00, 0xff));			// 屏幕清空为蓝色
	
			// 第二个“十”字, (210, 30)
			GUI_DrawHLine(200, 30, 220,RGB(0xff,0x00,0x00));// 在y=30位置处，画一条水平线200<x<220
			GUI_DrawVLine(210, 20, 40,RGB(0xff,0x00,0x00));	// 在x=210位置处，画一条垂直线20<y<40
	
			while(!Get_AD_XY());
			x1 = xData;
			y1 = yData;
			Lcd_ClearScr(RGB(0x00, 0x00, 0xff));			// 屏幕清空为蓝色
				
			// 第三个“十”字, (30, 290)
			GUI_DrawHLine(20, 290, 40,RGB(0xff,0x00,0x00));	// 在y=290位置处，画一条水平线20<x<40
			GUI_DrawVLine(30, 280, 300,RGB(0xff,0x00,0x00));// 在x=30位置处，画一条垂直线20<y<40
	
			while(!Get_AD_XY());
			x2 = xData;
			y2 = yData;
			Lcd_ClearScr(RGB(0x00, 0x00, 0xff));			// 屏幕清空为蓝色
				
			// 第四个“十”字, (210, 290)
			GUI_DrawHLine(200, 290, 220,RGB(0xff,0x00,0x00));// 在y=290位置处，画一条水平线20<x<40
			GUI_DrawVLine(210, 280, 300,RGB(0xff,0x00,0x00));// 在x=30位置处，画一条垂直线20<y<40
	
			while(!Get_AD_XY());
			x3 = xData;
			y3 = yData;
			Lcd_ClearScr(RGB(0x00, 0x00, 0xff));			// 屏幕清空为蓝色
				
			// 第五个“十”字, (120, 160)
			GUI_DrawHLine(110, 160, 130,RGB(0xff,0x00,0x00));	// 在y=290位置处，画一条水平线20<x<40
			GUI_DrawVLine(120, 150, 170,RGB(0xff,0x00,0x00));// 在x=30位置处，画一条垂直线20<y<40
	
			while(!Get_AD_XY());
			x4 = xData;
			y4 = yData;
			Lcd_ClearScr(RGB(0x00, 0x00, 0xff));			// 屏幕清空为蓝色
	
			// 通过四点坐标，计算平均值
			x_min = (x0+x2)/2;
			x_max = (x1+x3)/2;
			y_min = (y0+y1)/2;
			y_max = (y2+y3)/2;
					
//			Uart_Printf("(%d,%d), (%d,%d), (%d,%d), (%d,%d), (%d,%d)\n", x0, y0, x1, y1, x2, y2, x3, y3, x4, y4);	
//			Uart_Printf("4点平均值：(%d,%d), (%d,%d), (%d,%d), (%d,%d)\n", x_min, y_min, x_max, y_min, x_min, y_max, x_max, y_max);	
	
		}while(abs(x0-x2)>12||abs(x1-x3)>12||abs(y0-y1)>12||abs(y2-y3)>12||abs(x_min+x_max-x4*2)>20||abs(y_min+y_max-y4*2)>20);
	}
}

unsigned char Get_Touch( struct LcdDot *lcdXY ) 
{
	//无触摸，返回
	if( !Get_AD_XY() )
		return 0;

	(*lcdXY).x = 180*(int)(xData - x_min)/(int)(x_max - x_min)+30;
	(*lcdXY).y = 260*(int)(yData - y_min)/(int)(y_max - y_min)+30;

//	Uart_Printf("xData = %d, yData = %d\n", (*lcdXY).x, (*lcdXY).y);

    return 1;
}
