#include "Canvas.h"

/**
  *　新建并初始化一个画布
	*　返回初始化完毕的新画布
	*　返回类型struct Canvas*
**/
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

/**
  *　加入一张需要显示的图形
  *　
  *
**/
void addBitmap(struct Canvas *canvas,Bitmap bmp,uint16 x,uint16 y,uint16 width,uint16 height,char id[],int layer)
{
	struct BitmapsNode *new_bmp = (struct BitmapsNode *)malloc(sizeof(struct BitmapsNode));
	strcpy(new_bmp->id,id);
	new_bmp->bmp_data = bmp;
	new_bmp->layer = layer;
	new_bmp->x = x;new_bmp->y = y;
    new_bmp->width = width;new_bmp->height = height;
	new_bmp->next = NULL;
	
	canvas->bmps_tail->next = new_bmp;
	canvas->max_layer = (layer > canvas->max_layer)? layer: canvas->max_layer;
	canvas->bmps_tail = new_bmp;
	
}

/**
  *　删除编号为id的图像
  *　
  *
**/
void deleteBitmapById(struct Canvas *canvas,char id[])
{
	struct BitmapsNode *delete_pointer,*tmp_pointer;
	delete_pointer = canvas->bmps;
	while(delete_pointer->next != NULL)
	{
		tmp_pointer = delete_pointer->next;
		if( strcmp(tmp_pointer->id,id) == 0 )
		{
			delete_pointer->next = tmp_pointer->next;
			if(tmp_pointer == canvas->bmps_tail)
				canvas->bmps_tail = NULL;
			free(tmp_pointer);
		}
		break;
	}
	
}

/**
  *　清除整个画布（所有层）
  *　
  *
**/
void cleanCanvas(struct Canvas *canvas)
{
	struct BitmapsNode *cleaner,*clean_pointer;
	cleaner = canvas->bmps;
	while( cleaner->next != NULL)
	{
		clean_pointer = cleaner->next;
		free(cleaner);
		cleaner = clean_pointer;
	}
	free(canvas);
	canvas = NULL;
}

void update(struct Canvas *canvas)
{
	struct BitmapsNode *painter;
	int i= 0;
	painter = canvas->bmps;
	while(i <= canvas->max_layer)
	{
		while(painter->next != NULL )
		{
			if(painter->layer == i)
				Paint_Bmp(painter->x,painter->y,painter->width,painter->height,painter->bmp_data);
			painter = painter->next;
		}
		i++;
	}
}
