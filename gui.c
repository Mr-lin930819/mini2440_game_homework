#include "sys.h"
#include "LCD.h"
#include "adc.h"
#include "gui.h"
#include <math.h>
#include <string.h>


/**************************************************
 * �������ƣ�GetIcon
 * ����˵��������û�ѡ��Ĺ���ģ��
 * ���������*tmpXY -- ����������ֵ
 * ������������ص��ͼ����ֵ
***************************************************/ 
#define	ICON_X		3		// ͼ����ʾ����ʼX����
#define	ICON_Y		45		// ͼ����ʾ����ʼY����
#define	ICON_Width	78		// ͼ��Ŀ��
#define	ICON_High	78		// ͼ��ĸ߶�


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
		return 1 ;					// ��ˮ��
	}
	
	if( (*tmpXY).x>=(LEVEL_X+LEVEL_WIDTH*0) && (*tmpXY).x<=(LEVEL_X+LEVEL_WIDTH*1) &&	
		(*tmpXY).y>=LEVEL_2_Y && (*tmpXY).y<=(LEVEL_2_Y+LEVEL_HEIGHT))
	{
		return 2 ;					// ͼƬ��ʾ
	}
			
	if( (*tmpXY).x>=(LEVEL_X+LEVEL_WIDTH*0) && (*tmpXY).x<=(LEVEL_X+LEVEL_WIDTH*1) &&	
		(*tmpXY).y>=LEVEL_3_Y && (*tmpXY).y<=(LEVEL_3_Y+LEVEL_HEIGHT))
	{
		return 3 ;					// ����У��
	}
			
	if( (*tmpXY).x>=(LEVEL_X+LEVEL_WIDTH*0) && (*tmpXY).x<=(LEVEL_X+LEVEL_WIDTH*1) &&	
	    (*tmpXY).y>=LEVEL_4_Y && (*tmpXY).y<=(LEVEL_4_Y+LEVEL_HEIGHT*1))
	{
		return 4 ;					// ��ͼ
	}	
	
/*	
	if( (*tmpXY).x>=(ICON_X+ICON_Width*1) && (*tmpXY).x<=(ICON_X+ICON_Width*2) &&	
	    (*tmpXY).y>=(ICON_Y+ICON_High) && (*tmpXY).y<=(ICON_Y+ICON_High*2))
	{
		return 5 ;					// ƴ�����뷨
	}
		
	if( (*tmpXY).x>=(ICON_X+ICON_Width*2) && (*tmpXY).x<=(ICON_X+ICON_Width*3) &&		
	    (*tmpXY).y>=(ICON_Y+ICON_High) && (*tmpXY).y<=(ICON_Y+ICON_High*2))
	{
		return 6 ;					// ��̬ͼ��
	}
		
	if( (*tmpXY).x>=(ICON_X+ICON_Width*0) && (*tmpXY).x<=(ICON_X+ICON_Width*1) &&	
	    (*tmpXY).y>=(ICON_Y+ICON_High*2) && (*tmpXY).y<=(ICON_Y+ICON_High*3))
	{
		return 7 ;					// ���ü���
	}
		
	if( (*tmpXY).x>=(ICON_X+ICON_Width*1) && (*tmpXY).x<=(ICON_X+ICON_Width*2) &&	
	    (*tmpXY).y>=(ICON_Y+ICON_High*2) && (*tmpXY).y<=(ICON_Y+ICON_High*3))
	{
		return 8 ;					// RFID��Ƶ��
	}
	if( (*tmpXY).x>=(ICON_X+ICON_Width*2) && (*tmpXY).x<=(ICON_X+ICON_Width*3) &&	
	    (*tmpXY).y>=(ICON_Y+ICON_High*2) && (*tmpXY).y<=(ICON_Y+ICON_High*3))
	{
		return 9 ;					// ϵͳ˵��
	}
	*/

	return 0;
}

/**************************************************
 * �������ƣ�GUI_DrawVLine
 * ����˵�������ƴ�ֱֱ��
 * ���������Leftֱ�ߵ���߽�, Topֱ�ߵ��ϱ߽�
 *			 Bottomֱ�ߵ��±߽�, Colorֱ�ߵ���ɫ
 * ���������0--�����������ȷ, 1--�ɹ�
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
 * �������ƣ�GUI_DrawHLine
 * ����˵��������ˮƽֱ��
 * ���������Leftֱ�ߵ���߽�, Topֱ�ߵ��ϱ߽�
 *			 Rightֱ�ߵ��ұ߽�, Colorֱ�ߵ���ɫ
 * ���������0--�����������ȷ, 1--�ɹ�
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
 * �������ƣ�GUI_DrawRect
 * ����˵�������ƾ��ο�
 * ���������(X0, Y0)���ο�����Ͻ�����
 *			 (X1, Y1)���ο�����½�����
 *			 Colorֱ�ߵ���ɫ
 * ���������0--�����������ȷ, 1--�ɹ�
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
 * �������ƣ�GUI_DrawLine
 * ����˵��������ֱ��
 * ���������(X0, Y0)ֱ�ߵ����Ͻ�����
 *			 (X1, Y1)ֱ�ߵ����½�����
 *			 Colorֱ�ߵ���ɫ
 * ���������0--�����������ȷ, 1--�ɹ�
***************************************************/
uint8 GUI_DrawLine(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Color)
{
   int32   dx;						// ֱ��x���ֵ����
   int32   dy;          			// ֱ��y���ֵ����
   int8    dx_sym;					// x����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
   int8    dy_sym;					// y����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
   int32   dx_x2;					// dx*2ֵ���������ڼӿ������ٶ�
   int32   dy_x2;					// dy*2ֵ���������ڼӿ������ٶ�
   int32   di;						// ���߱���
   
	if(X0>=LCD_WIDTH||X1>=LCD_WIDTH||Y0>=LCD_HEIGHT||Y1>=LCD_HEIGHT)
		return 0;
   	
	dx = X1-X0;						// ��ȡ����֮��Ĳ�ֵ
	dy = Y1-Y0;
	
	if(dx>0)						//�ж��������򣬻��Ƿ�Ϊˮƽ�ߡ���ֱ�ߡ���
		dx_sym = 1;					//dx>0������dx_sym=1
	else if(dx<0)
		dx_sym = (int8)-1;			//dx<0������dx_sym=-1
	else if(dx==0)					//dx=0������ֱ�ߣ���һ��
		return GUI_DrawVLine(X0, Y0, Y1, Color);
   
	if(dy>0)						//�ж�y�᷽��
		dy_sym = 1;					//dy>0������dy_sym=1
	else if(dy<0)
		dy_sym = (int8)-1;			//dy<0������dy_sym=-1
	else if(dy==0)					//dy==0����ˮƽ�ߣ���һ��
		return GUI_DrawHLine(X0, Y0, X1, Color);
    
	dx *=dx_sym;					// ��dx��dyȡ����ֵ 
	dy *=dy_sym;
	dx_x2 =dx*2;					// ����2����dx��dyֵ 
	dy_x2 =dy*2;
   
	// ʹ��Bresenham�����л�ֱ��
	if(dx>=dy)						// ����dx>=dy����ʹ��x��Ϊ��׼
	{
		di = dy_x2 - dx;
		while(X0!=X1)
		{
			PutPixel(X0, Y0, Color);
			X0 += dx_sym;
			if(di<0)
				di += dy_x2;		// �������һ���ľ���ֵ
			else
			{
				di += dy_x2 - dx_x2;
				Y0 += dy_sym;
			}
		}							// ��ʾ���һ��
		PutPixel(X0, Y0, Color);
	}
	else							// ����dx<dy����ʹ��y��Ϊ��׼
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
		}							// ��ʾ���һ��
		PutPixel(X0, Y0, Color);
	}	
	return 1;
}

/**************************************************
 * �������ƣ�GUI_FillRect
 * ����˵��������ɫ������
 * ���������(X1, Y1)ֱ�ߵ����Ͻ�����
 *			 (X2, Y2)ֱ�ߵ����½�����
 *			 Colorֱ�ߵ���ɫ
 * �����������
***************************************************/
void GUI_FillRect(uint16 X1, uint16 Y1, uint16 X2, uint16 Y2, uint16 Color)
{
    int i;

    for(i=Y1;i<=Y2;i++)
		GUI_DrawHLine(X1,i,X2,Color);
}
