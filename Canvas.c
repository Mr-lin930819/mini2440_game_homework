#include "Canvas.h"

/**
  *　新建并初始化一个画布
	*　返回初始化完毕的新画布
	*　返回类型struct Canvas*
**/

/*
struct Canvas* createEmptyCanvas()
{
	struct Canvas* newCanvas = (struct Canvas*)malloc( sizeof(struct Canvas) );
	Bitmaps bmps_head = (Bitmaps)malloc( sizeof(struct BitmapsNode) );
	bmps_head->bmp_data = NULL;
	strcpy(bmps_head->id,"a0");
	bmps_head->layer = 0;
	bmps_head->next = NULL;
	newCanvas->bmps = bmps_head;
	newCanvas->bmps_tail = bmps_head;
	newCanvas->max_layer = 0;
	return newCanvas;
}
*/

/***********( addBitmap )************ *
 * 参数：@canvas —— 画布					    *
 *			@x、@y、@width、@height —— 加  *
 *			入的图片位置大小信息			  *
 *			@id —— 图片的标识字符串		  *
 *			@layer —— 图片的层					  *
 * 返回值：无										  	  *
 * 功能：将需要显示的图片加入到画 *
 *			布中。											  *
 ************************************ */
void addBitmap(Canvas canvas,Bitmap bmp,uint16 x,uint16 y,uint16 width,uint16 height,char id[],int layer)
{
	int i;
	for(i=0;i<30;i++)
	{
		if(strcmp(canvas[i].id,"")==0)
			break;
	}
	strcpy(canvas[i].id,id);
	canvas[i].bmp_data = bmp;
	canvas[i].layer = layer;
	canvas[i].x = x;
	canvas[i].y = y;
	canvas[i].width = width;
	canvas[i].height = height;
	
}


/********( deleteBitmapById )******** *
 * 参数：@canvas —— 画布				 		 	*
 *			@id —— 图片的标识字符串		  *
 * 返回值：无										  		*
 * 功能：将指定的图片从画布中删去 *
 ************************************ */
void deleteBitmapById(Canvas canvas,char id[])
{
	int i;
	for(i=0;i<30;i++)
	{
		if(strcmp(canvas[i].id,id)==0){
			strcpy(canvas[i].id,"");
			canvas[i].bmp_data = NULL;
			canvas[i].layer = 0;
			canvas[i].x = 0;
			canvas[i].y = 0;
			canvas[i].width = 0;
			canvas[i].height = 0;
			Uart_Printf("Bitmap(%s)deleted!\n",id);
			break;
		}
	}
}

/********( cleanCanvas )********* *
 * 参数：@canvas —— 画布				  *
 * 返回值：无										  *
 * 功能：清除所有在画布上的图  *
 ******************************** */
void cleanCanvas(Canvas canvas)
{
	int i;
	for(i=0;i<30;i++)
	{
		strcpy(canvas[i].id,"");
		canvas[i].bmp_data = NULL;
		canvas[i].layer = 0;
		canvas[i].x = 0;
		canvas[i].y = 0;
		canvas[i].width = 0;
		canvas[i].height = 0;
	}
}

/***********( update )*********** *
 * 参数：@canvas —— 画布				  *
 * 返回值：无										  *
 * 功能：绘制所有在画布上的图  *
 ******************************** */
void update(Canvas canvas)
{
	int i;
	struct BitmapsNode *painter;
	for(i=0;i<30;i++)
	{
		painter = &canvas[i];
		if(strcmp(painter->id,"")!=0)
			Paint_Bmp(painter->x,painter->y,painter->width,painter->height,painter->bmp_data);
	}
}
