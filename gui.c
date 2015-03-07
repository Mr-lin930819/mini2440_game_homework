#include "sys.h"
#include "LCD.h"
#include "adc.h"
#include "gui.h"
#include <math.h>
#include <string.h>


/**************************************************
 * 函数名称：GetIcon
 * 描述说明：获得用户选择的功能模块
 * 输入参数：*tmpXY -- 触摸屏像素值
 * 输出参数：返回点击图标编号值
***************************************************/ 
#define	ICON_X		3		// 图标显示的起始X坐标
#define	ICON_Y		45		// 图标显示的起始Y坐标
#define	ICON_Width	78		// 图标的宽度
#define	ICON_High	78		// 图标的高度


#define LEVEL_WIDTH 100
#define LEVEL_HEIGHT 35
#define LEVEL_X 70
#define LEVEL_1_Y 85
#define LEVEL_2_Y 138
#define LEVEL_3_Y 188
#define LEVEL_4_Y 238

unsigned char GetIcon( struct LcdDot *tmpXY) 
{
	if( (*tmpXY).x>=LEVEL_X && (*tmpXY).x<=(LEVEL_X+LEVEL_WIDTH*1) &&	
		(*tmpXY).y>=LEVEL_1_Y && (*tmpXY).y<=(LEVEL_1_Y+LEVEL_HEIGHT))
	{
		return 1 ;					// 流水灯
	}
	
	if( (*tmpXY).x>=(LEVEL_X+LEVEL_WIDTH*0) && (*tmpXY).x<=(LEVEL_X+LEVEL_WIDTH*1) &&	
		(*tmpXY).y>=LEVEL_2_Y && (*tmpXY).y<=(LEVEL_2_Y+LEVEL_HEIGHT))
	{
		return 2 ;					// 图片显示
	}
			
	if( (*tmpXY).x>=(LEVEL_X+LEVEL_WIDTH*0) && (*tmpXY).x<=(LEVEL_X+LEVEL_WIDTH*1) &&	
		(*tmpXY).y>=LEVEL_3_Y && (*tmpXY).y<=(LEVEL_3_Y+LEVEL_HEIGHT))
	{
		return 3 ;					// 坐标校正
	}
			
	if( (*tmpXY).x>=(LEVEL_X+LEVEL_WIDTH*0) && (*tmpXY).x<=(LEVEL_X+LEVEL_WIDTH*1) &&	
	    (*tmpXY).y>=LEVEL_4_Y && (*tmpXY).y<=(LEVEL_4_Y+LEVEL_HEIGHT*1))
	{
		return 4 ;					// 画图
	}	
	
/*	
	if( (*tmpXY).x>=(ICON_X+ICON_Width*1) && (*tmpXY).x<=(ICON_X+ICON_Width*2) &&	
	    (*tmpXY).y>=(ICON_Y+ICON_High) && (*tmpXY).y<=(ICON_Y+ICON_High*2))
	{
		return 5 ;					// 拼音输入法
	}
		
	if( (*tmpXY).x>=(ICON_X+ICON_Width*2) && (*tmpXY).x<=(ICON_X+ICON_Width*3) &&		
	    (*tmpXY).y>=(ICON_Y+ICON_High) && (*tmpXY).y<=(ICON_Y+ICON_High*2))
	{
		return 6 ;					// 动态图标
	}
		
	if( (*tmpXY).x>=(ICON_X+ICON_Width*0) && (*tmpXY).x<=(ICON_X+ICON_Width*1) &&	
	    (*tmpXY).y>=(ICON_Y+ICON_High*2) && (*tmpXY).y<=(ICON_Y+ICON_High*3))
	{
		return 7 ;					// 复用键盘
	}
		
	if( (*tmpXY).x>=(ICON_X+ICON_Width*1) && (*tmpXY).x<=(ICON_X+ICON_Width*2) &&	
	    (*tmpXY).y>=(ICON_Y+ICON_High*2) && (*tmpXY).y<=(ICON_Y+ICON_High*3))
	{
		return 8 ;					// RFID射频卡
	}
	if( (*tmpXY).x>=(ICON_X+ICON_Width*2) && (*tmpXY).x<=(ICON_X+ICON_Width*3) &&	
	    (*tmpXY).y>=(ICON_Y+ICON_High*2) && (*tmpXY).y<=(ICON_Y+ICON_High*3))
	{
		return 9 ;					// 系统说明
	}
	*/

	return 0;
}

/**************************************************
 * 函数名称：GUI_DrawVLine
 * 描述说明：绘制垂直直线
 * 输入参数：Left直线的左边界, Top直线的上边界
 *			 Bottom直线的下边界, Color直线的颜色
 * 输出参数：0--输入参数不正确, 1--成功
***************************************************/ 
uint8 GUI_DrawVLine(uint16 Left,uint16 Top,uint16 Bottom,uint16 Color)
{
	uint16 Temp;
	
	if(Top>Bottom)
	{
		Temp=Top;
		Top=Bottom;
		Bottom=Temp;
	}

	if(Left>=LCD_WIDTH||Bottom>=LCD_HEIGHT)
		return 0;

	for(Temp=Top;Temp<=Bottom;Temp++)
        PutPixel(Left,Temp,Color);
        
    return 1;
}

/**************************************************
 * 函数名称：GUI_DrawHLine
 * 描述说明：绘制水平直线
 * 输入参数：Left直线的左边界, Top直线的上边界
 *			 Right直线的右边界, Color直线的颜色
 * 输出参数：0--输入参数不正确, 1--成功
***************************************************/
uint8 GUI_DrawHLine(uint16 Left,uint16 Top,uint16 Right,uint16 Color)
{
	uint16 Temp;
	
	if(Left>Right)
	{
		Temp=Left;
		Left=Right;
		Right=Temp;
	}
		
	if(Right>=LCD_WIDTH||Top>=LCD_HEIGHT)
		return 0;
	
	for(Temp=Left;Temp<=Right;Temp++)
        PutPixel(Temp,Top,Color);
        
    return 1;
}

/**************************************************
 * 函数名称：GUI_DrawRect
 * 描述说明：绘制矩形框
 * 输入参数：(X0, Y0)矩形框的左上角坐标
 *			 (X1, Y1)矩形框的右下角坐标
 *			 Color直线的颜色
 * 输出参数：0--输入参数不正确, 1--成功
***************************************************/
uint8 GUI_DrawRect(uint16 X0, uint16 Y0, uint16 X1, uint16 Y1, uint16 Color)
{
	if(!GUI_DrawHLine(X0, Y0, X1, Color)) return 0;
	if(!GUI_DrawHLine(X0, Y1, X1, Color)) return 0;
	if(!GUI_DrawVLine(X0, Y0, Y1, Color)) return 0;
	if(!GUI_DrawVLine(X1, Y0, Y1, Color)) return 0;
	
	return 1;
}

/**************************************************
 * 函数名称：GUI_DrawLine
 * 描述说明：绘制直线
 * 输入参数：(X0, Y0)直线的左上角坐标
 *			 (X1, Y1)直线的右下角坐标
 *			 Color直线的颜色
 * 输出参数：0--输入参数不正确, 1--成功
***************************************************/
uint8 GUI_DrawLine(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Color)
{
   int32   dx;						// 直线x轴差值变量
   int32   dy;          			// 直线y轴差值变量
   int8    dx_sym;					// x轴增长方向，为-1时减值方向，为1时增值方向
   int8    dy_sym;					// y轴增长方向，为-1时减值方向，为1时增值方向
   int32   dx_x2;					// dx*2值变量，用于加快运算速度
   int32   dy_x2;					// dy*2值变量，用于加快运算速度
   int32   di;						// 决策变量
   
	if(X0>=LCD_WIDTH||X1>=LCD_WIDTH||Y0>=LCD_HEIGHT||Y1>=LCD_HEIGHT)
		return 0;
   	
	dx = X1-X0;						// 求取两点之间的差值
	dy = Y1-Y0;
	
	if(dx>0)						//判断增长方向，或是否为水平线、垂直线、点
		dx_sym = 1;					//dx>0，设置dx_sym=1
	else if(dx<0)
		dx_sym = (int8)-1;			//dx<0，设置dx_sym=-1
	else if(dx==0)					//dx=0，画垂直线，或一点
		return GUI_DrawVLine(X0, Y0, Y1, Color);
   
	if(dy>0)						//判断y轴方向
		dy_sym = 1;					//dy>0，设置dy_sym=1
	else if(dy<0)
		dy_sym = (int8)-1;			//dy<0，设置dy_sym=-1
	else if(dy==0)					//dy==0，画水平线，或一点
		return GUI_DrawHLine(X0, Y0, X1, Color);
    
	dx *=dx_sym;					// 将dx、dy取绝对值 
	dy *=dy_sym;
	dx_x2 =dx*2;					// 计算2倍的dx及dy值 
	dy_x2 =dy*2;
   
	// 使用Bresenham法进行画直线
	if(dx>=dy)						// 对于dx>=dy，则使用x轴为基准
	{
		di = dy_x2 - dx;
		while(X0!=X1)
		{
			PutPixel(X0, Y0, Color);
			X0 += dx_sym;
			if(di<0)
				di += dy_x2;		// 计算出下一步的决策值
			else
			{
				di += dy_x2 - dx_x2;
				Y0 += dy_sym;
			}
		}							// 显示最后一点
		PutPixel(X0, Y0, Color);
	}
	else							// 对于dx<dy，则使用y轴为基准
	{
		di = dx_x2 - dy;
		while(Y0!=Y1)
		{
			PutPixel(X0, Y0, Color);
			Y0 += dy_sym;
			if(di<0)
				di += dx_x2;
			else
			{
				di += dx_x2 - dy_x2;
				X0 += dx_sym;
			}
		}							// 显示最后一点
		PutPixel(X0, Y0, Color);
	}	
	return 1;
}

/**************************************************
 * 函数名称：GUI_FillRect
 * 描述说明：用颜色填充矩形
 * 输入参数：(X1, Y1)直线的左上角坐标
 *			 (X2, Y2)直线的右下角坐标
 *			 Color直线的颜色
 * 输出参数：无
***************************************************/
void GUI_FillRect(uint16 X1, uint16 Y1, uint16 X2, uint16 Y2, uint16 Color)
{
    int i;

    for(i=Y1;i<=Y2;i++)
		GUI_DrawHLine(X1,i,X2,Color);
}
