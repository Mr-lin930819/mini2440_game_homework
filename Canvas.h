#ifndef _CANVAS_H_
#define _CANVAS_H_
#include "LCD.h"
#include <stdlib.h>
#include <string.h>

typedef unsigned char * Bitmap;

struct BitmapsNode{
	Bitmap bmp_data;
	uint16 x;
	uint16 y;
	uint16 width;
	uint16 height;
	int layer;
	char id[5];
	struct BitmapsNode *next;
};

typedef struct BitmapsNode *Bitmaps;

struct Canvas{
	Bitmaps bmps;
	struct BitmapsNode *bmps_tail;
	int max_layer;
};	

/*
 *
 * 绘图的接口函数
 *
 */
struct Canvas* createEmptyCanvas(void);
void addBitmap(struct Canvas *canvas,Bitmap bmp,uint16 x,uint16 y,char id[],int layer);
void deleteBitmapById(struct Canvas *canvas,char id[]);
void cleanCanvas(struct Canvas *canvas);
void update(struct Canvas *canvas);	

#endif
